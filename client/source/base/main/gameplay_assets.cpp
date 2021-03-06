#include "pch.h"
#include "gameplay_assets.h"






//
// PropertiesAsset
//

Cache< PropertiesAsset > * PropertiesAsset::_cache = nullptr;

PropertiesAsset::PropertiesAsset()
    : _properties(NULL)
{
}

PropertiesAsset::~PropertiesAsset()
{
    SAFE_DELETE(_properties);
}

PropertiesAsset * PropertiesAsset::create(const char * url)
{
    gameplay::Properties * props = gameplay::Properties::create(url);
    if (!props)
        return NULL;

    PropertiesAsset * res = new PropertiesAsset();
    res->_properties = props;
    res->setURL(url);

    return res;
}

bool PropertiesAsset::reload()
{
    gameplay::Properties * props = gameplay::Properties::create(getURL());
    if (!props)
        return false;

    SAFE_DELETE(_properties);
    _properties = props;

    return true;
}



//
// SpriteBatchAsset
//

Cache< SpriteBatchAsset > * SpriteBatchAsset::_cache = nullptr;

SpriteBatchAsset::SpriteBatchAsset()
    : _spriteBatch(NULL)
{
}

SpriteBatchAsset::~SpriteBatchAsset()
{
    SAFE_DELETE(_spriteBatch);
}

SpriteBatchAsset * SpriteBatchAsset::create(const char * url)
{
    gameplay::Properties * properties = gameplay::Properties::create(url);
    if (!properties)
        return NULL;

    gameplay::Material * material = gameplay::Material::create((strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace());
    SAFE_DELETE(properties);
    if (!material)
        return NULL;

    SpriteBatchAsset * res = new SpriteBatchAsset();
    res->_spriteBatch = gameplay::SpriteBatch::create(material);
    res->setURL(url);

    SAFE_RELEASE(material);

    return res;
}

bool SpriteBatchAsset::reload()
{
    gameplay::Properties * properties = gameplay::Properties::create(getURL());
    if (!properties)
        return false;

    gameplay::Material * material = gameplay::Material::create((strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace());
    SAFE_DELETE(properties);
    if (!material)
        return false;

    SAFE_DELETE(_spriteBatch);
    _spriteBatch = gameplay::SpriteBatch::create(material);
    SAFE_RELEASE(material);

    return true;
}
