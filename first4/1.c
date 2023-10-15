#include <stdio.h>

int main() {
    char* filename = "example.bin";

    FILE* file = fopen(filename, "wb"); // запись в бинарном режиме

    if (file == NULL) 
    {
        printf("Не удалось создать файл, грустно 😥\n");
        return 1;
    }

    // 1 байт = 8 бит -> 0-255 -> unsigned char
    unsigned char bytes[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    size_t cnt = fwrite(bytes, sizeof(unsigned char), sizeof(bytes) / sizeof(unsigned char), file);
    /*
    ptr ->  указатель на блок данных, которые хотим записать
    size_t -> размер одного элемента наших данных
    size_t n -> количество элементов (размера size_t) в нашем блоке 
    file -> указатель на файл для записи
    */ 
    printf("📝 Записано %ld/%ld элементов\n", cnt, sizeof(bytes) / sizeof(unsigned char));
    fclose(file);


    file = fopen(filename, "rb"); // чтение в бинарном режиме

    if (file == NULL) 
    {
        printf("Не удалось открыть файл, грустно 😥\n");
        return 1;
    }

    unsigned char byte;
    cnt = 0;
    while (fread(&byte, sizeof(unsigned char), 1, file) == 1) {
        /*
        ptr ->  указатель на блок данных, в который запишутся данные
        size_t -> размер одного элемента наших данных
        size_t n -> количество элементов (размера size_t) в файле, которые считаем
        file -> указатель на файл для чтения
        */ 
        ++cnt;
        printf("\n%d\n", byte);
        printf("_fileno %d\n", fileno(file));
        printf("_flags %08x\n", file->_flags);
        printf("_IO_read_ptr %p\n", file->_IO_read_ptr);
        printf("_IO_read_end %p\n", file->_IO_read_end);
        printf("_IO_read_base %p\n", file->_IO_read_base);
        printf("_IO_write_base %p\n", file->_IO_write_base);
        printf("_IO_write_ptr %p\n", file->_IO_write_ptr);
        printf("_IO_write_end %p\n", file->_IO_write_end);
        printf("_IO_buf_base %p\n", file->_IO_buf_base);
        printf("_IO_buf_end %p\n", file->_IO_buf_end);
        printf("_IO_save_base %p\n", file->_IO_save_base);
        printf("_IO_backup_base %p\n", file->_IO_backup_base);
        printf("_IO_save_end %p\n", file->_IO_save_end);
        printf("_markers %p\n", file->_markers);
        printf("_chain %p\n", file->_chain);
        printf("_fileno %d\n", file->_fileno);
        printf("_flags2 %08x\n", file->_flags2);
        printf("_old_offset %lld\n", file->_old_offset);
        printf("_cur_column %d\n", file->_cur_column);
        printf("_vtable_offset %d\n", file->_vtable_offset);
        printf("_shortbuf %s\n", file->_shortbuf);
        printf("_lock %p\n", file->_lock);
        printf("_offset %lld\n", file->_offset);
        printf("_codecvt %p\n", file->_codecvt);
        printf("_wide_data %p\n", file->_wide_data);
        printf("_freeres_list %p\n", file->_freeres_list);
        printf("_freeres_buf %p\n", file->_freeres_buf);
        printf("__pad5 %p\n", file->__pad5);
        printf("_mode %d\n", file->_mode);
        printf("_unused2 %s\n", file->_unused2);
    }
    printf("\n📄 Считано %ld элементов\n\n", cnt);
    fclose(file); 

    file = fopen(filename, "rb"); // чтение в бинарном режиме

    if (file == NULL) 
    {
        printf("Не удалось открыть файл, грустно 😥\n");
        return 1;
    }

    fseek(file, 3, SEEK_SET);
    /*
    file -> указатель на файл для установки текущей позиции при чтении/записи
    offset -> смещение относительно whence
    whence -> место относительной позиции: начало, конец и текущая позиция в файле
    */

    unsigned char buffer[4];
    cnt = fread(buffer, sizeof(unsigned char), sizeof(buffer) / sizeof(unsigned char), file);
    printf("Считанные байты: ");
    for (size_t i = 0; i < cnt; ++i) 
    {
        printf("%d ", buffer[i]);
    }
    printf("\n📄 Считано %ld/%ld элементов\n", cnt, sizeof(buffer) / sizeof(unsigned char));
    fclose(file);

    return 0;
}
