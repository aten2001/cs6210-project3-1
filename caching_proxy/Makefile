PROJECT_ROOT_DIR := ..
BUILD_DIR := build

SERVER_SOURCES := \
	CachingProxyServer.cpp

SERVER_OBJECTS := \
	$(BUILD_DIR)/CachingProxyServer.o

CLIENT_SOURCES := \
	CachingProxyClient.cpp

CLIENT_OBJECTS := \
	$(BUILD_DIR)/CachingProxyClient.o

TARGETS := \
	CachingProxyServer \
	CachingProxyClient

GEN_CPP_SOURCES = \
	gen-cpp/CachingProxy.cpp \
	gen-cpp/CachingProxy.h \
	gen-cpp/caching_proxy_constants.cpp \
	gen-cpp/caching_proxy_constants.h \
	gen-cpp/caching_proxy_types.cpp \
	gen-cpp/caching_proxy_types.h \
	gen-cpp/CachingProxy_server.skeleton.cpp

GEN_CPP_OBJECTS = \
    $(BUILD_DIR)/CachingProxy.o \
    $(BUILD_DIR)/caching_proxy_constants.o \
    $(BUILD_DIR)/caching_proxy_types.o

WEB_PAGE_CACHE_SOURCES = \
    $(PROJECT_ROOT_DIR)/web_page_cache/WebPageCache.cpp \
    $(PROJECT_ROOT_DIR)/web_page_cache/FifoReplPolicy.cpp

WEB_PAGE_CACHE_OBJECTS = \
    $(BUILD_DIR)/WebPageCache.o \
    $(BUILD_DIR)/FifoReplPolicy.o

CPPFLAGS = -O3 -std=c++11
LDFLAGS = -lthrift -lboost_program_options
CURL_LIBS = $(shell curl-config --libs)
CURL_CFLAGS = $(shell curl-config --cflags)

.PHONY: all clean

all: $(TARGETS)

CachingProxyServer: $(SERVER_OBJECTS) $(GEN_CPP_OBJECTS) $(WEB_PAGE_CACHE_OBJECTS)
	g++ -o $@ $^ $(LDFLAGS) $(CURL_LIBS)

$(BUILD_DIR)/CachingProxyServer.o : CachingProxyServer.cpp $(GEN_CPP_SOURCES)
	g++ -o $@ -c $< $(CPPFLAGS) $(CURL_CFLAGS)

CachingProxyClient: $(CLIENT_OBJECTS) $(GEN_CPP_OBJECTS)
	g++ -o $@ $^ $(LDFLAGS)

$(GEN_CPP_SOURCES) : caching_proxy.thrift
	thrift --gen cpp -r $<

$(WEB_PAGE_CACHE_OBJECTS): $(BUILD_DIR)/%.o : $(PROJECT_ROOT_DIR)/web_page_cache/%.cpp
	g++ -o $@ -c $< $(CPPFLAGS)

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
