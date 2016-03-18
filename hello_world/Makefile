BUILD_DIR := build

SERVER_SOURCES := \
	HelloWorldServer.cpp

SERVER_OBJECTS := \
	$(BUILD_DIR)/HelloWorldServer.o

CLIENT_SOURCES := \
	HelloWorldClient.cpp

CLIENT_OBJECTS := \
	$(BUILD_DIR)/HelloWorldClient.o

TARGETS := \
	HelloWorldServer \
	HelloWorldClient

GEN_CPP_SOURCES = \
	gen-cpp/HelloWorld.cpp \
	gen-cpp/HelloWorld.h \
	gen-cpp/hello_world_constants.cpp \
	gen-cpp/hello_world_constants.h \
	gen-cpp/hello_world_types.cpp \
	gen-cpp/hello_world_types.h \
	gen-cpp/HelloWorld_server.skeleton.cpp

GEN_CPP_OBJECTS = \
    $(BUILD_DIR)/HelloWorld.o \
    $(BUILD_DIR)/hello_world_constants.o \
    $(BUILD_DIR)/hello_world_types.o \

CPPFLAGS = -O3
LDFLAGS = -lthrift -lboost_program_options

.PHONY: all clean

all: $(TARGETS)

HelloWorldServer: $(SERVER_OBJECTS) $(GEN_CPP_OBJECTS)
	g++ -o $@ $^ $(LDFLAGS)

HelloWorldClient: $(CLIENT_OBJECTS) $(GEN_CPP_OBJECTS)
	g++ -o $@ $^ $(LDFLAGS)

$(GEN_CPP_SOURCES) : hello_world.thrift
	thrift --gen cpp -r $<

$(BUILD_DIR)/%.o : %.cpp $(GEN_CPP_SOURCES)
	g++ -o $@ -c $< $(CPPFLAGS)

$(BUILD_DIR)/%.o : gen-cpp/%.cpp $(GEN_CPP_SOURCES)
	g++ -o $@ -c $< $(CPPFLAGS)

$(CLIENT_OBJECTS) : | $(BUILD_DIR)
$(GEN_CPP_OBJECTS) : | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f -v $(BUILD_DIR)/* gen-cpp/* $(TARGETS)
