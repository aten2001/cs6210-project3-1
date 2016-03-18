BUILD_DIR := build

SERVER_SOURCES := \
	SimpleProxyServer.cpp

SERVER_OBJECTS := \
	$(BUILD_DIR)/SimpleProxyServer.o

CLIENT_SOURCES := \
	SimpleProxyClient.cpp

CLIENT_OBJECTS := \
	$(BUILD_DIR)/SimpleProxyClient.o

TARGETS := \
	SimpleProxyServer \
	SimpleProxyClient

GEN_CPP_SOURCES = \
	gen-cpp/SimpleProxy.cpp \
	gen-cpp/SimpleProxy.h \
	gen-cpp/simple_proxy_constants.cpp \
	gen-cpp/simple_proxy_constants.h \
	gen-cpp/simple_proxy_types.cpp \
	gen-cpp/simple_proxy_types.h \
	gen-cpp/SimpleProxy_server.skeleton.cpp

GEN_CPP_OBJECTS = \
    $(BUILD_DIR)/SimpleProxy.o \
    $(BUILD_DIR)/simple_proxy_constants.o \
    $(BUILD_DIR)/simple_proxy_types.o \

CPPFLAGS = -O3
LDFLAGS = -lthrift -lboost_program_options
CURL_LIBS = $(shell curl-config --libs)
CURL_CFLAGS = $(shell curl-config --cflags)

.PHONY: all clean

all: $(TARGETS)

SimpleProxyServer: $(SERVER_OBJECTS) $(GEN_CPP_OBJECTS)
	g++ -o $@ $^ $(LDFLAGS) $(CURL_LIBS)

$(BUILD_DIR)/SimpleProxyServer.o : SimpleProxyServer.cpp $(GEN_CPP_SOURCES)
	g++ -o $@ -c $< $(CPPFLAGS) $(CURL_CFLAGS)

SimpleProxyClient: $(CLIENT_OBJECTS) $(GEN_CPP_OBJECTS)
	g++ -o $@ $^ $(LDFLAGS)

$(GEN_CPP_SOURCES) : simple_proxy.thrift
	thrift --gen cpp -r $<

$(BUILD_DIR)/%.o : %.cpp $(GEN_CPP_SOURCES)
	g++ -o $@ -c $< $(CPPFLAGS)

$(BUILD_DIR)/%.o : gen-cpp/%.cpp $(GEN_CPP_SOURCES)
	g++ -o $@ -c $< $(CPPFLAGS)

$(CLIENT_OBJECTS) : | $(BUILD_DIR)
$(SERVER_OBJECTS) : | $(BUILD_DIR)
$(GEN_CPP_OBJECTS) : | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f -v $(BUILD_DIR)/* gen-cpp/* $(TARGETS)
