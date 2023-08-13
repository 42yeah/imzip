// A simple image zipping utility. It resizes image and cut them in half if their resolution is crazy high, then save it using 50% JPEG compression.
#include <iostream>
#include <image.h>


int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cerr << "Usage: " << argv[0] << " <image to compress ...>" << std::endl;
        return 1;
    }

    int errors = 0;
    for (int i = 1; i < argc; i++)
    {
        Image image;

        if (!image.load(argv[i]))
        {
            std::cerr << "ERROR: cannot open " << argv[i] << std::endl;
            errors++;
            continue;
        }

        // Image is more than 2k - resize it first by cutting it in half
        while (image.w >= 2560 && image.h >= 1440)
        {
            if (!image.resize(image.w / 2, image.h / 2))
            {
                std::cerr << "ERROR: cannot resize: " << argv[i] << std::endl;
                errors++;
                continue;
            }
        }

        if (!image.save_compressed(std::string(argv[i]) + "_cmpr.jpg", 50))
        {
            std::cerr << "ERROR: cannot save: " << argv[i] << std::endl;
            errors++;
            continue;
        }

        std::cout << argv[i] << std::endl;
    }

    return errors;
}
