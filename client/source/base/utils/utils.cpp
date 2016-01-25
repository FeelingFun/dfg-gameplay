#include "pch.h"
#include "utils.h"
#include "ui/dial_button.h"
#include "ui/expanded_tab.h"
#include "utf8.h"

#if defined (WIN32)
#include <Rpc.h>
#elif defined (__ANDROID__) || defined (__EMSCRIPTEN__)
#include <uuidlib/uuid.h>
#else
#include <uuid/uuid.h>
#endif





std::string Utils::generateUUID( )
{
    std::string s;

#ifdef WIN32
    UUID uuid;
    UuidCreate ( &uuid );

    unsigned char * str;
    UuidToStringA ( &uuid, &str );

    s = ( const char* ) str;

    RpcStringFreeA ( &str );
#else
    uuid_t uuid;
    uuid_generate_random ( uuid );
    char str[37];
    uuid_unparse ( uuid, str );

    s = str;
#endif

    return s;
}




const wchar_t * Utils::UTF8ToWCS(const char * str)
{
    static std::wstring res;
    res.clear();
    utf8::unchecked::utf8to16(str, str + strlen(str), std::back_inserter(res));
    return res.c_str();
}



const char * Utils::WCSToUTF8(const wchar_t * str)
{
    static std::string res;
    res.clear();
    utf8::unchecked::utf16to8(str, str + wcslen(str), std::back_inserter(res));
    return res.c_str();
}




const wchar_t * Utils::ANSIToWCS(const char * str)
{
    static wchar_t result[2048];

    wchar_t * o = result;
    while( *str )
        *o++ = *str++;
    *o = 0;

    return result;
}




const char * Utils::format(const char * fmt, ...)
{
    static char results[16][2048];
    static int resInd = 0;

    char * result = results[resInd];
    resInd = (resInd + 1) & 15;

    va_list args;
    va_start(args, fmt);

#ifdef WIN32
    _vsnprintf(result, 2048, fmt, args);
#else
    vsnprintf(result, 2048, fmt, args);
#endif

    va_end(args);

    return result;
}


const char * Utils::urlEncode(const char * src)
{
    static std::string res[16];
    static int resInd = 0;

    std::string& result = res[resInd];
    resInd = (resInd + 1) & 15;

    result.clear();

    static char hexmap[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    int max = strlen(src);
    for (int i = 0; i < max; i++, src++)
    {
        if (('0' <= *src && *src <= '9') ||
            ('A' <= *src && *src <= 'Z') ||
            ('a' <= *src && *src <= 'z') ||
            (*src == '~' || *src == '-' || *src == '_' || *src == '.')
            )
        {
            result.push_back(*src);
        }
        else
        {
            result.push_back('%');
            result.push_back(hexmap[(unsigned char)(*src) >> 4]);
            result.push_back(hexmap[*src & 0x0F]);
        }
    }

    return result.c_str();
}



const wchar_t * Utils::clipTextToBounds(const wchar_t * text, float width, const gameplay::Font * font, float fontSize)
{
    static std::wstring result;

    if (width <= 0.0f)
        return L"";

    float textw = 0, texth = 0;
    font->measureText(text, fontSize, gameplay::Font::LEFT_TO_RIGHT, &textw, &texth);

    if (textw < width)
        return text;

    result = text;

    result.erase(result.end() - 1, result.end());
    result.push_back('.');
    result.push_back('.');
    result.push_back('.');
    do
    {
        result.erase(result.end() - 4, result.end());
        result.push_back('.');
        result.push_back('.');
        result.push_back('.');
        font->measureText(result.c_str(), fontSize, gameplay::Font::LEFT_TO_RIGHT, &textw, &texth);
    } while (result.size() > 3 && textw >= width);

    return result.c_str();
}

const wchar_t * Utils::clipTextToBounds(const wchar_t * text, float width, float height, const gameplay::Font * font, float fontSize)
{
    static std::wstring result;

    if (width <= 0.0f || height <= fontSize)
        return L"";
    
    gameplay::Rectangle clip(width, height);
    gameplay::Rectangle out;

    font->measureText(text, clip, fontSize, gameplay::Font::LEFT_TO_RIGHT, &out, gameplay::Font::ALIGN_TOP_LEFT, true, true);

    if (out.width < width && out.height < height)
        return text;

    result = text;

    result.erase(result.end() - 1, result.end());
    result.push_back('.');
    result.push_back('.');
    result.push_back('.');
    do
    {
        result.erase(result.end() - 4, result.end());
        result.push_back('.');
        result.push_back('.');
        result.push_back('.');
        font->measureText(result.c_str(), clip, fontSize, gameplay::Font::LEFT_TO_RIGHT, &out, gameplay::Font::ALIGN_TOP_LEFT, true, true);
    } while (result.size() > 3 && (out.width >= width || out.height >= height));

    return result.c_str();
}


void Utils::serializeString(gameplay::Stream * stream, const std::string& str)
{
    int32_t size = static_cast<int32_t>(str.size());
    stream->write(&size, sizeof(size), 1);
    stream->write(str.c_str(), sizeof(char), size);
}

void Utils::deserializeString(gameplay::Stream * stream, std::string * str)
{
    int32_t size = 0;
    if (stream->read(&size, sizeof(size), 1) != 1)
        return;

    if (size < 0 || size > 65535)
        return; // something wrong with data

    char * buf = reinterpret_cast<char *>(alloca(sizeof(char)* (size + 1)));
    if (buf)
    {
        stream->read(buf, sizeof(char), size);
        buf[size] = '\0';
        if (str)
        {
            str->clear();
            *str = buf;
        }
    }
}

void Utils::scaleUIControl(gameplay::Control * control, float kx, float ky)
{
    if (!control)
        return;

    // the actual scaling
    if (!control->isXPercentage())
        control->setX(control->getX() * kx);
    if (!control->isYPercentage())
        control->setY(control->getY() * ky);
    if (!control->isWidthPercentage() && (control->getAutoSize() & gameplay::Control::AUTO_SIZE_WIDTH) == 0)
        control->setWidth(control->getWidth() * kx);
    if (!control->isHeightPercentage() && (control->getAutoSize() & gameplay::Control::AUTO_SIZE_HEIGHT) == 0)
        control->setHeight(control->getHeight() * ky);

    const gameplay::Theme::Border& border = control->getBorder();
    const gameplay::Theme::Margin& margin = control->getMargin();
    const gameplay::Theme::Padding& padding = control->getPadding();
    control->setBorder(border.top * ky, border.bottom * ky, border.left * kx, border.right * kx);
    control->setMargin(margin.top * ky, margin.bottom * ky, margin.left * kx, margin.right * kx);
    control->setPadding(padding.top * ky, padding.bottom * ky, padding.left * kx, padding.right * kx);

    control->setFontSize(ky * control->getFontSize());
    control->setCharacterSpacing(ky * control->getCharacterSpacing());
    control->setLineSpacing(ky * control->getLineSpacing());

    if (strcmp(control->getTypeName(), "Slider") == 0)
        static_cast<gameplay::Slider *>(control)->setScaleFactor(ky);

    if (strcmp(control->getTypeName(), "ImageControl") == 0)
    {
        gameplay::ImageControl * image = static_cast< gameplay::ImageControl * >(control);
        const gameplay::Rectangle& dstRegion = image->getRegionDst();
        image->setRegionDst(dstRegion.x * kx, dstRegion.y * ky, dstRegion.width * kx, dstRegion.height * ky);
    }
    
    if (strcmp(control->getTypeName(), "DialButton") == 0)
    {
        DialButton * button = static_cast<DialButton *>(control);
        button->setHeightCollapsed(ky * button->getHeightCollapsed());
        button->setHeightExpanded(ky * button->getHeightExpanded());
    }
    
    if (strcmp(control->getTypeName(), "ExpandedTab") == 0)
    {
        ExpandedTab * tab = static_cast<ExpandedTab * >(control);
        tab->setWidthMinimized(kx * tab->getWidthMinimized());
        tab->setWidthMaximized(kx * tab->getWidthMaximized());
    }

    if (control->isContainer())
    {
        gameplay::Container * container = static_cast<gameplay::Container *>(control);
        container->setScrollScale(container->getScrollScale() * ky);

        const std::vector< gameplay::Control * >& children = container->getControls();
        for (unsigned j = 0; j < children.size(); j++)
            scaleUIControl(children[j], kx, ky);
    }
}


void Utils::measureChildrenBounds(gameplay::Container * container, float * width, float * height)
{
    // Calculate total width and height.
    float totalWidth = 0.0f, totalHeight = 0.0f;
    const std::vector<gameplay::Control*>& controls = container->getControls();
    for (size_t i = 0, count = controls.size(); i < count; ++i)
    {
        gameplay::Control* control = controls[i];

        if (!control->isVisible())
            continue;

        const gameplay::Rectangle& bounds = control->getBounds();
        const gameplay::Theme::Margin& margin = control->getMargin();

        float newWidth = bounds.x + bounds.width + margin.right;
        if (newWidth > totalWidth)
            totalWidth = newWidth;

        float newHeight = bounds.y + bounds.height + margin.bottom;
        if (newHeight > totalHeight)
            totalHeight = newHeight;
    }

    if (width)
        *width = totalWidth;
    if (height)
        *height = totalHeight;
}