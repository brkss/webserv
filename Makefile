DEBUG = false
NAME = webserv

SRC = ./ConfigParse/srcs/lexer.cpp \
	  ./ConfigParse/srcs/token.cpp \
	  ./ConfigParse/srcs/server.cpp \
	  ./ConfigParse/srcs/location.cpp \
	  ./ConfigParse/srcs/http.cpp \
	  ./ConfigParse/srcs/parse.cpp \
	  ./ConfigParse/srcs/defaultConfig.cpp \
	  ./Server/ServerDrive.cpp \
	  ./Server/Client.cpp \
	  ./Server/HttpRequest.cpp \
	  ./Server/Utils.cpp \
	  ./Server/ErrorHandler.cpp \
	  ./Server/Colors.cpp \
	  ./Response/Response.cpp \
	  ./Handler/Handler.cpp \
	  ./Handler/CGI.cpp \
	  ./Handler/HandlerUtils.cpp \
	  ./Response/ResponseUtils.cpp \
	  main.cpp

OBJS=${SRC:.cpp=.o} 

FLAGS = -Wall -Werror -Wextra -fsanitize=address

COMPILER = c++

STANDARD = -std=c++98

all: $(NAME)

%.o: %.cpp
	$(COMPILER) $(FLAGS) -c -D DEBUG=$(DEBUG) $< -o ${<:.cpp=.o}

$(NAME): $(OBJS)
	$(COMPILER) $(FLAGS) $(STANDARD) $(OBJS) -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

