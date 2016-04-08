rpsGame: main.cpp
	g++ -o postfixCalc main.cpp PostFixCalculator.cpp -std=c++0x

LIBS = -lboost_unit_test_framework

test:	PostFixCalculator.cpp PostFixCalculator.t.cpp
	g++ -o test PostFixCalculator.cpp PostFixCalculator.t.cpp $(LIBS)

clean:
	$(RM) postfixCalc test
