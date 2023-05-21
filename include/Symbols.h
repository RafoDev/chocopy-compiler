vector<string> keywords = {
		"False", "None", "True", "and", "as", "assert", "async", "await", "bool", "break", "class", "continue", "def", "del", "elif", "else", "except", "finally", "for", "from", "global", "if", "import", "in", "is", "int", "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try", "while", "with", "yield"};
vector<string> operators = {
		"+", "-", "*", "%", "/", "<",
		">", "<=", ">=", "==", "!="};
vector<string> delimiters = {
		")",
		"(",
		"[",
		"]",
		",",
		".",
		"=",
		":",
		"->"};

regex idRegexp("[a-z|A-Z][a-z|A-Z|0-9|_]*");
regex numberRegexp("[0-9]+");
regex escapeRegexp("[\"'ntrbfv\\0]");
