# Scanner test 
# This is a comment. Next lines should be ignored
# Testing integers
0
2147483647 # maximum integer allowed
2147483648 # lexical error
00678 # this is not a valid number
# Testing strings
"Hello" # string Hello
"Hell\o" # \o is not valid
" \"Hello \" “ # string “Hello"
' Hello' # ' is not valid but Hello is id regex 
# Testing ids and keywords
thisisanunusuallylongidentifierbutitislegal
forTrue
for True
x10 temp_1
v@r # invalid
wait await
False
None
# testing operators and expressions
1+1
(x:int) -> bool
1==0 and 3<=2
2%2? #unknown character
([ ) ]
# testing indentation
# A broken program
def is_even(x:int) -> bool:
 if x % 2.0 == 1:
 return 05 # FIXME
 else:
 return True;
print(is_even("3")) # FIXME
# Search in a list
def contains(items:[int], x:int) -> bool:
 i:int = 0
 while i < len(items):
 if items[i] == x:
 return True
 i = i + 1
 return False
if contains([4, 8, 15, 16, 23], 15):
 print("Item found!") # Prints this
else:
 print("Item not found.")