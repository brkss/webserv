NAME = webserv

SRC = ./config/ConfigParse/srcs/lexer.cpp \
	  ./config/ConfigParse/srcs/token.cpp \
	  ./config/ConfigParse/srcs/server.cpp \
	  ./config/ConfigParse/srcs/location.cpp \
	  ./config/ConfigParse/srcs/http.cpp \
	  ./config/ConfigParse/srcs/parse.cpp \
	  ./config/ConfigParse/srcs/defaultConfig.cpp \
	  main.cpp
	  #./Server/ServerDrive.cpp \

FLAGS = -Wall -Werror -Wextra -fsanitize=address

COMPILER = c++

STANDARD = -std=c++98

all: $(NAME)

$(NAME): $(SRC)
	$(COMPILER) $(FLAGS) $(STANDARD) $(SRC) -o $@

clean:
	rm -rf $(NAME)

re: clean all

