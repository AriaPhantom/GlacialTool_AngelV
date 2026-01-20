#pragma once
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void copy_and_rename_file(const std::string& original_path, const std::string& new_filename) {
    try {
        // 创建一个路径对象
        fs::path original_file(original_path);

        // 获取原文件所在的目录
        fs::path target_directory = original_file.parent_path();

        // 创建新文件的完整路径
        fs::path new_file = target_directory / new_filename;

        // 复制文件
        fs::copy_file(original_file, new_file, fs::copy_options::overwrite_existing);

        std::cout << "File copied and renamed successfully!" << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }
}