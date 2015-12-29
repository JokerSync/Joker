/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QString>
#include <QRegExpValidator>
#include <QList>
#include <QStack>
#include <QByteArray>

#include <QDomDocument>
#include <QDomNodeList>
#include <QtXml>
#include <QXmlStreamWriter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QImage>
#include <QColor>

#include <QSettings>
#include <QMetaEnum>

#include <boost/version.hpp>
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#include <boost/format.hpp>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
