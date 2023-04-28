NAME = server

SRC = ./config/ConfigParse/lerxer.cpp \
	  ./config/ConfigParse/lexer_test.cpp \
	  ./config/ConfigParse/token.cpp


FLAGS = -Wall -Werror -Wextra

COMPILER = c++

STANDARD = -std=c++98

all: $(NAME)

$(NAME): $(SRC)
	$(COMPILER) $(FLAGS) $(STANDARD) $(SRC) -o $@

clean:
	rm -rf $(NAME)

re: clean all

