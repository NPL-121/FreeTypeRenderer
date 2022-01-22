/* Небольшой пример использования Freetype. */
/* Пример сделан на основе примера из документации к Freetype */
/* Отрисуем строку в текстовый файл. */ /* FreeType 2 library. */

#include <ft2build.h>
#include FT_FREETYPE_H

#include <fstream>
#include <iostream>
#include <string>

using std::string;
using std::wstring;
using std::cout;
using std::cin;

FT_Library ft;
FT_Face face;

std::ofstream file;

int main()
{
    setlocale(LC_ALL, "ru_RU.utf8");

    string font_file_name = "Arimo-Regular.ttf"; // путь к шрифту
    wstring draw_str;      // юникод текст для отрисовки
    string render_to_file_name;// файл для сохранения отрисованной строки

    // ! Инициализация библиотеки Freetype 
    if(FT_Init_FreeType(&ft)) 
    { 
        cout<<"Could not init freetype library\n";
        return 1;
    }

    // ! Загрузка шрифта 
    if(FT_New_Face(ft, font_file_name.c_str(), 0, &face))
    { // ! Не удалось загрузить 
        cout << "Could not " << font_file_name << std::endl;
        return 1;
    }

    // ! Просим ввести текст для отрисовки 
    cout<<"Enter text to render: \n";
    std::getline(std::wcin, draw_str);
    std::wcout << draw_str << "\n";

    // ! Просим ввести путь к файлу для сохранения 
    // cout<<"Enter file to save render result: \n";
    //std::getline(std::cin, render_to_file_name);
    render_to_file_name = "render";

    // ! Открываем файл для записи (или создаем, если его нет)
    file.open(render_to_file_name.c_str(), std::ios_base::out);
    if(!file) 
    {
        cout<<"Could open file: " << render_to_file_name << "\n";
        return 1;
    }

    // ! Установка размера пикселя (специфическое понятие в Freetype) 
    // и обязательное для FT_Load_Glyph 
    FT_Set_Pixel_Sizes(face, 0, 36);

    // ! Указатель на глиф 
    FT_GlyphSlot g = face->glyph;

    // ! Цикл отрисовки образов символов из строки в файл 
    for(unsigned int i = 0; i < draw_str.size(); ++i)
    {
        // Грузим образ одного символа
        if (FT_Load_Char(face, draw_str.at(i), FT_LOAD_RENDER))
        {
            // Загрузка не удалась!
            cout << "glyph not load! \n";
            continue;
        }

        // Отрисовка образа в файл
        for (int y = 0; y < g->bitmap.rows; ++y)
        {
            for (int i = 0; i < g->bitmap.width; ++i)
            {
                unsigned char c = g->bitmap.buffer[y*g->bitmap.width + i];
                if (c == 0)
                    file << ' '; // если ноль записываем пробел;
                else if (c < 128)
                    file << '+'; // если "прозрачность" ниже 128
                else
                    file << '#'; // если "прозрачность" выше 128
            }
            file << '\n';
        }
        file << "\n\n";
    }

    file.flush();
    file.close();

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return 0;
}
