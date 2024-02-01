# 定義編譯器
CXX = g++

# -wall 可讓 errors 全部印出來
# 可以在這裡加優化（-01, -02...）
#CXXFLAGS = -std=c++14 -Wall 
# 可以在這裡指定版本
CXXFLAGS = -Wall -std=c++11 -Iinclude -MP -MD

BUILD_DIR=build
# 執行檔名
TARGET = exec

# 以 SRCS 代表所有 .cpp 檔案
SRCS = $(wildcard *.cpp)



# 將 .cpp 替換成 .o 
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPFILES = $(patsubst %.o,%.d,$(OBJS))

	

all: $(TARGET)

# 連結檔案（link）
# 白話文：g++ -wall -o exec *.cpp
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# 編譯檔案（compile）到 build資料夾
# 代表所有 .o depend on 對應的 .cpp
# $@ = 目標文件, $< = 第一個依賴的檔案（應該可以用$^）
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPFILES)
