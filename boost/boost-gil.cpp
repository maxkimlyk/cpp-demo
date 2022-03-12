#include <boost/gil.hpp>
#include <boost/gil/extension/io/bmp.hpp>

#include <demo-common.h>

const std::string_view kCheckersFileName = "checkers.bmp";

DEMO(gen_image)
{
    const size_t width = 400;
    const size_t height = 300;
    const boost::gil::rgb8_pixel_t black_pixel(10, 80, 30);
    const boost::gil::rgb8_pixel_t white_pixel(245, 175, 225);

    boost::gil::rgb8_image_t img(width, height);
    auto view = boost::gil::view(img);

    for (size_t y = 0; y < height; ++y)
    {
        auto iter = view.row_begin(y);
        for (size_t x = 0; x < width; ++x)
        {
            const size_t yy = y / 8;
            const size_t xx = x / 8;
            iter[x] = (xx + yy) % 2 ? black_pixel : white_pixel;
        }
    }

    boost::gil::write_view(kCheckersFileName.data(), view, boost::gil::bmp_tag{});
    std::cout << "Generated: " << kCheckersFileName << std::endl;
}

DEMO(histogram)
{
    boost::gil::rgb8_image_t input_image;
    boost::gil::read_image(kCheckersFileName.data(), input_image, boost::gil::bmp_tag{});

    std::vector<int> histogram(256, 0);
    auto gray_view = boost::gil::color_converted_view<boost::gil::gray8_pixel_t>(boost::gil::view(input_image));

    for (auto pixel : gray_view)
    {
        ++histogram[pixel];
    }

    std::cout << "Non-zero values:" << std::endl;
    for (size_t i = 0; i < 256; ++i) {
      if (histogram[i]) {
        std::cout << i << ": " << histogram[i] << std::endl;
      }
    }
}


