#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void encrypte(char *filename) {
  FILE *file;
  file = fopen(filename, "rb");
  if (file == NULL) {
    printf("FILE OPEN FAILED");
    return 1;
  }

  // 读取二进制数据
  fseek(file, 0, SEEK_END);     // 将文件指针移动到文件末尾
  long file_size = ftell(file); // 获取文件大小
  fseek(file, 0, SEEK_SET);     // 将文件指针移动回文件开头

  char *buffer = (char *)malloc(file_size); // 分配内存以存储文件内容
  if (buffer == NULL) {
    printf("内存分配失败\n");
    fclose(file);
    return 1;
  }

  fread(buffer, 1, file_size, file); // 读取文件内容
  fclose(file);

  // 复杂加密 buffer
  for (int i = 0; i < file_size; i++) {
    buffer[i] = buffer[i] ^ 0x12;
  }

  FILE *output_file = fopen("encrypted", "wb");
  if (output_file == NULL) {
    printf("无法打开文件 %s\n", filename);
    return 1;
  }
  fwrite(buffer, 1, file_size, output_file);
  printf("文件 %s\n", filename);
  fclose(output_file);
  free(buffer);
}

void decrypte(char *targetfile) {
  FILE *file;
  file = fopen(targetfile, "rb");
  if (file == NULL) {
    printf("FILE OPEN FAILED");
    return 1;
  }

  fseek(file, 0, SEEK_END);     // 将文件指针移动到文件末尾
  long file_size = ftell(file); // 获取文件大小
  fseek(file, 0, SEEK_SET);     // 将文件指针移动回文件开头

  char *buffer = (char *)malloc(file_size); // 分配内存以存储文件内容
  if (buffer == NULL) {
    printf("内存分配失败\n");
    fclose(file);
    return 1;
  }

  fread(buffer, 1, file_size, file); // 读取文件内容
  fclose(file);

  // 解密 buffer
  for (int i = 0; i < file_size; i++) {
    buffer[i] = buffer[i] ^ 0x12;
  }

  FILE *output_file = fopen("decrypted.zip", "wb");
  if (output_file == NULL) {
    printf("无法打开文件 %s\n", file);
    return 1;
  }
  fwrite(buffer, 1, file_size, output_file);
  printf("文件解密\n");
  fclose(output_file);
  free(buffer);
}

int main(int argc, char *argv[]) {
  int c;
  char *filename, *output_file;

  // 检查参数个数
  if (argc < 2) {
    printf("Usage: %s -d file <command>\n", argv[0]);
    return 1;
  }

  // 选择命令
  while ((c = getopt(argc, argv, "e:d:")) != -1) {
    switch (c) {
    case 'e':
      filename = optarg;
      encrypte(filename);
      break;
    case 'd':
      output_file = optarg;
      decrypte(output_file);
      break;
    default:
      printf("Usage:\n %s -e file (encrypte)\n %s -d file (decrypte)\n",
             argv[0], argv[0]);
      return 1;
    }
  }

  return 0;
}
