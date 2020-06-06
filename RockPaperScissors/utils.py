from enum import EnumMeta


class ListEnumMeta(EnumMeta):
    def __str__(cls):
        result = f"{cls.__name__.lower()}s are: ["

        for member in cls:
            result += f"{str(member).split('.')[1]}, "

        result = result[:-2]
        result += "]"

        return result
