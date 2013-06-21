#include "extension_qt/web_view_creator.h"

#include "webdriver_session.h"
#include "webdriver_logging.h"

#include "web_view_util.h"
#include "extension_qt/widget_view_handle.h"

namespace webdriver {

QWebViewCreator::QWebViewCreator() {}

bool QWebViewCreator::CreateViewByClassName(const Logger& logger, const std::string& className, ViewHandle** view) const {
	ViewHandle* handle = NULL;

    if (factory.empty())
        return false;

	if (className.empty()) {
		// get first found QWebView
        CreateViewMethod createMethod = factory.begin()->second;
        handle = new QViewHandle(static_cast<QWidget*>(createMethod()));
	} else {
    	FactoryMap::const_iterator it = factory.find(className);
        if (it != factory.end())
        {
        	CreateViewMethod createMethod = it->second;
            handle = new QViewHandle(static_cast<QWidget*>(createMethod()));
        }
    }

    if (NULL != handle) {
        QWidget* widget = (dynamic_cast<QViewHandle*>(handle))->get();
        std::string objClassName(widget->metaObject()->className());

        if (NULL != widget) {
            widget->show();
        
            logger.Log(kInfoLogLevel, "QWebViewCreator created view ("
                            + objClassName +") by class name - "+className);

            *view = handle;

            return true;
        } else {
            logger.Log(kSevereLogLevel, "QWebViewCreator, smth wrong.");
            handle->Release();
        }
    }

    // view was not created
    return false;
}

bool QWebViewCreator::CreateViewForUrl(const Logger& logger, const std::string& url, ViewHandle** view) const {
    if (factory.empty())
        return false;

    if (!QWebViewUtil::isUrlSupported(url))
        return false;

    ViewHandle* handle = NULL;
    // get first found QWebView
    CreateViewMethod createMethod = factory.begin()->second;
    handle = new QViewHandle(static_cast<QWidget*>(createMethod()));

    if (NULL != handle) {
        QWidget* widget = (dynamic_cast<QViewHandle*>(handle))->get();
        std::string objClassName(widget->metaObject()->className());

        if (NULL != widget) {
            widget->show();
        
            logger.Log(kInfoLogLevel, "QWebViewCreator created view("
                        + objClassName + ") by url - " + url);

            *view = handle;

            return true;
        } else {
            logger.Log(kSevereLogLevel, "QWebViewCreator, smth wrong.");
            handle->Release();
        }
    }

	return false;
};



} // namespace webdriver