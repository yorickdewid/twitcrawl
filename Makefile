NAME = twitcrawl
SRC = $(NAME).cpp
LDFLAGS += -lcurl
CC = g++
REMOVE = rm -f

all: target

target:
	$(CC) -Wall -o $(NAME) twitcurl.cpp oauthlib.cpp urlencode.cpp base64.cpp HMAC_SHA1.cpp SHA1.cpp $(SRC) $(LDFLAGS)

clean:
	$(REMOVE) $(NAME)
	$(REMOVE) *.o

