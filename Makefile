SDIR = src
ODIR = obj
RES = shaders textures
OSX = osx
SOURCE = $(wildcard $(SDIR)/*.cpp)
OBJS = $(patsubst $(SDIR)%.cpp, $(ODIR)%.o,$(SOURCE))
CFLAGS = -std=c++11 -Wall
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

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(ODIR)
	$(CXX) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $^ $(LIBS) -o $@

ifeq ($(OS),Darwin)
MacApp: $(TARGET) $(OBJS)
	@mkdir -p $(MAC_TARGET).app/Contents/MacOS
	@mkdir -p $(MAC_TARGET).app/Contents/Resources
	@mv $(TARGET) $(MAC_TARGET).app/Contents/MacOS
	@cp -R $(RES) $(MAC_TARGET).app/Contents/Resources
	@cp $(OSX)/Info.plist $(MAC_TARGET).app/Contents
	@cp $(OSX)/$(MAC_TARGET).icns $(MAC_TARGET).app/Contents/Resources
	@echo $(MAC_TARGET).app created
else
MacApp:
	@echo "MacApp is not supported by your system. You must be running OS X"
endif

clean:
	@rm -rf $(ODIR) $(TARGET) $(MAC_TARGET).app 
