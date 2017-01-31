SDIR = src
ODIR = obj
RES = shaders
OSX = osx
SOURCE = $(wildcard $(SDIR)/*.cpp)
OBJS = $(patsubst $(SDIR)%.cpp, $(ODIR)%.o,$(SOURCE))
CFLAGS = -Wall
CC = gcc
CXX = g++
TARGET = pong
MAC_TARGET = Pong

OS = $(shell uname)
ifeq ($(OS),Darwin)
    LIBS = -framework SDL2 -framework OpenGL -lglew
	CFLAGS += -I /System/Library/Frameworks/OpenGL.framework/Headers
	ifneq (,$(findstring MacApp,$(MAKECMDGOALS)))
		CFLAGS += -D MacApp
		LIBS += -framework CoreFoundation
		OSX_SOURCE = $(wildcard $(OSX)/*.cpp)
		SOURCE += $(OSX_SOURCE)
		OBJ += $(patsubst $(OSX)%.cpp, $(ODIR)%.o,$(OSX_SOURCE))
	endif
else
    LIBS = -lSDL2 -lGL -lGLEW
endif

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $^ $(LIBS) -o $@

ifeq ($(OS),Darwin)
MacApp: $(TARGET) $(OBJS)
	mkdir -p $(MAC_TARGET).app/Contents/MacOS
	mkdir $(MAC_TARGET).app/Contents/Resources
	mv $(TARGET) $(MAC_TARGET).app/Contents/MacOS
	cp -R $(RES) $(MAC_TARGET).app/Contents/Resources
	cp $(OSX)/Info.plist $(MAC_TARGET).app/Contents
	CP $(OSX)/$(MAC_TARGET).icns $(MAC_TARGET).app/Contents/Resources
else
MacApp:
	@echo "MacApp is not supported by your system. You must be running OS X"
endif

$(ODIR)/%.o: $(SDIR)/%.cpp $(ODIR)
	$(CXX) $(CFLAGS) -c -o $@ $<

$(ODIR):
	mkdir -p $(ODIR)

clean:
	rm -rf $(ODIR) $(TARGET) $(MAC_TARGET).app 
