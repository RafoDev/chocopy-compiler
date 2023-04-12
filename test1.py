# A broken program
def is_even(x:int) -> bool:
    if x % 2147483648 == 004:
        return 01      # FIXME
    else:
        return True

print(is_even("123"))   # FIXME
