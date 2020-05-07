import argparse
import pandas as pd
from pathlib import Path

import yfinance as yf
from forex_python.converter import CurrencyRates
import functools

from mftool import Mftool

import subprocess
import shutil
import pprint
import datetime
import copy

import smtplib
from os.path import basename
from email.mime.application import MIMEApplication
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.utils import COMMASPACE, formatdate

pd.options.mode.chained_assignment = None  # default='warn'

SMTP_SERVER = "smtp.gmail.com"
SMTP_PORT = 587
SMTP_USERNAME = "ankushbajoria007@gmail.com"
SMTP_PASSWORD = "rqffvzoknjfwyhxe"


# https://stackoverflow.com/questions/44356693/pprint-with-custom-float-formats
class FormatPrinter(pprint.PrettyPrinter):

    def __init__(self, formats):
        super(FormatPrinter, self).__init__()
        self.formats = formats

    def format(self, obj, ctx, maxlvl, lvl):
        if type(obj) in self.formats:
            return self.formats[type(obj)] % obj, 1, 0
        return pprint.PrettyPrinter.format(self, obj, ctx, maxlvl, lvl)


def send_mail(send_to, subject, body, type="text", files=None):
    assert isinstance(send_to, list)

    msg = MIMEMultipart()
    msg['From'] = SMTP_USERNAME 
    msg['To'] = COMMASPACE.join(send_to)
    msg['Date'] = formatdate(localtime=True)
    msg['Subject'] = subject

    msg.attach(MIMEText(body, type))

    for f in files or []:
        with open(f, "rb") as fil:
            part = MIMEApplication(
                fil.read(),
                Name=basename(f)
            )
        # After the file is closed
        part['Content-Disposition'] = 'attachment; filename="%s"' % basename(f)
        msg.attach(part)


    smtp = smtplib.SMTP(SMTP_SERVER, SMTP_PORT)

    smtp.ehlo()
    smtp.starttls()
    smtp.login(SMTP_USERNAME, SMTP_PASSWORD)
    smtp.sendmail(SMTP_USERNAME, send_to, msg.as_string())
    smtp.close()


@functools.lru_cache(maxsize=16)
def get_conversion(ccy: str):
    ccy_rates = CurrencyRates()
    return ccy_rates.get_rate("USD", ccy)


def get_last_close(symbol: str, t: str):
    if t == "stock":
        ticker = yf.Ticker(symbol)
        history = ticker.history()
        history = history[history.Volume != 0]
        last_close = history.iloc[-1].Close
        return last_close
    
    assert (t == "mf")

    return float(Mftool().get_scheme_quote(symbol)['nav'])


def update_prices(portfolio: pd.DataFrame) -> pd.DataFrame:
    cost_basis = 0
    mark_to_market = 0

    current_prices = []
    pnls = []
    notionals = []
    invested_amounts = []
    for i,row in portfolio.iterrows():
        last_close = get_last_close(row["symbol"], row["type"])
        current_conversion = get_conversion(row["currency"])
        cost_basis += row["buy_price"] * row["position"] / current_conversion
        mark_to_market += last_close * row["position"] / current_conversion
        notionals.append(last_close * row["position"] / current_conversion)
        invested_amounts.append(row["buy_price"] * row["position"] / current_conversion)
 
        current_prices.append(last_close)
        pnls.append(row["position"] * (last_close - row["buy_price"]) / current_conversion)

    portfolio["current_price"] = current_prices
    portfolio["notional"] = notionals
    portfolio["invested"] = invested_amounts
    portfolio["unrealized_pnl"] = pnls

    total = {
        "type": "summary",    
        "symbol": "total", 
        "invested": cost_basis, 
        "currency": "USD",
        "notional": mark_to_market,
        "unrealized_pnl": mark_to_market - cost_basis}

    return portfolio, total


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--portfolio", required=True, type=str)
    parser.add_argument("--mail", action="store_true")

    args = parser.parse_args()

    portfolio_path = Path(args.portfolio)

    if not portfolio_path.exists():
        raise FileNotFoundError(portfolio_path)

    portfolio = pd.read_csv(portfolio_path)
    
    portfolio, total = update_prices(portfolio[portfolio.symbol!="total"])

    cols = ["type", "desc", "category", "symbol", "currency", "position", "buy_price",
            "current_price", "invested", "notional", "unrealized_pnl", "category_return", 
            "return"]

    portfolio.sort_values("unrealized_pnl", ascending=False, inplace=True)
    portfolio.reset_index(drop=True, inplace=True)
    portfolio["symbol"] = [str(x) for x in portfolio["symbol"]]

    bond_df = portfolio[portfolio.category == "bond"]
    equity_df = portfolio[portfolio.category == "equity"]

    bond_return = (bond_df.notional.sum() - bond_df.invested.sum()) / bond_df.invested.sum() * 100
    equity_return = (equity_df.notional.sum() - equity_df.invested.sum()) / equity_df.invested.sum() * 100
    portfolio["category_return"] = [bond_return if c == "bond" else equity_return for c in portfolio["category"]]

    bond_percentage = bond_df.notional.sum() / total["notional"] * 100
    equity_percentage = equity_df.notional.sum() / total["notional"] * 100

    total["desc"] = f"bond: {bond_percentage:.2f}%, equity: {equity_percentage:.2f}%"

    portfolio = portfolio.append(total, ignore_index=True)
    portfolio["return"] = (portfolio["notional"] - portfolio["invested"]) / portfolio["invested"] * 100

    shutil.copy2(str(portfolio_path), f"{portfolio_path}.bk")
    portfolio[cols].to_csv(portfolio_path, float_format="%.1f", index=False)

    mail_total = copy.copy(total)
    mail_total["invested"] = f"{total['invested']:.2f}"
    mail_total["notional"] = f"{total['notional']:.2f}"
    mail_total["unrealized_pnl"] = f"{total['unrealized_pnl']:.2f}"

    if args.mail:
        send_mail(send_to=["ankushbajoria007@gmail.com"],
                  subject=f"portfolio {datetime.date.today()}",
                  body=portfolio.to_html(float_format=lambda x: '%.2f' % x),
                  type='html')

    return subprocess.run(["open", str(portfolio_path)]).returncode if not args.mail else 0


if __name__ == "__main__":
    exit(main())
