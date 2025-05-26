# 使用到的第三方库
- jsoncpp
- MySQL C API 8.0
# 注意事项
安装完成上述库后，CMakeLists.txt文件中需使用`include_directories`指定上述两个库的头文件目录，以及使用`link_directories`指定上述两个库的动态库目录   
