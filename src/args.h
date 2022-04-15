#include <iostream>
namespace args{
    //Two dimensional int
    typedef struct{
        int x, y;
    } vector2d; //Used as a "cleaner" way to select the size of the korai window

    inline vector2d extractSizes(char * text)
    {
        //Gets the sizes from a string in the form <width>x<height>
        //Finds an 'x' and then all the numbers before it are the width and all the numbers after it are the height
        int xpos{0};
        for(int i{0}; i < sizeof(text); i++)
        {
            if (text[i] == 'x')
            {
                xpos = i;
            }
        }
        vector2d r;
        std::string tmp{""};
        for(int i{0}; i < xpos; i++)
        {
            tmp += text[i];
        }
        r.x = std::stoi(tmp);
        tmp.clear();

        for(int i{xpos + 1}; i < sizeof(text); i++)
        {
            tmp += text[i];
        }
        r.y = std::stoi(tmp);
        return r;
    }
}