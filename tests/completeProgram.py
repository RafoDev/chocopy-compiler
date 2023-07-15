def add(x: int, y: int) -> int:
    return x + y

def main():
    a = 5
    b = 3
    result = add(a, b)
    if result > 0:
        print("The result is positive.")
    elif result < 0:
        print("The result is negative.")
    else:
        print("The result is zero.")
    
    while a > 0:
        print("Counting down:", a)
        a = 1
    
    for i in range(3):
        print("Current number:", i)

    pass

if __name__ == "__main__":
    main()