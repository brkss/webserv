NAME = server

SRC = ./config/ConfigParse/lexer.cpp \
	  ./config/ConfigParse/main.cpp \
	  ./config/ConfigParse/token.cpp \
	  ./config/ConfigParse/server.cpp \
	  ./config/ConfigParse/location.cpp \
	  ./config/ConfigParse/http.cpp \
	  ./config/ConfigParse/parse.cpp \
	  ./config/ConfigParse/defaultConfig.cpp

FLAGS = -Wall -Werror -Wextra -fsanitize=address

COMPILER = c++

STANDARD = -std=c++98

all: $(NAME)

$(NAME): $(SRC)
	$(COMPILER) $(FLAGS) $(STANDARD) $(SRC) -o $@

clean:
	rm -rf $(NAME)

re: clean all

