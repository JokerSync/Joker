#include <unistd.h>

#include <QString>
#include <QByteArray>

#include "PhTools/PhDebug.h"

#include "PhSandbox.h"

#import <Foundation/NSURL.h>

QByteArray createSecurityScopeBookmark(QString fileName)
{
	QString urlString = QString("file://%1").arg(fileName);
	NSString *nsUrlString = [NSString stringWithUTF8String:urlString.toStdString().c_str()];
	NSURL *url = [NSURL URLWithString:nsUrlString];

	NSError *error = nil;
	NSData *data = [url bookmarkDataWithOptions:NSURLBookmarkCreationWithSecurityScope
			includingResourceValuesForKeys:nil
			relativeToURL:nil
			error:&error];

	if(error)
		PHERR << QString::fromNSString([error localizedDescription]);

	return QByteArray::fromNSData(data);
}

void resolveSecurityScopeBookmark(QString fileName, QByteArray bookmark)
{
	if(access(fileName.toStdString().c_str(), R_OK) == 0) {
		PHDEBUG << "already read access";
		return;
	}

	if(bookmark.length() <= 0) {
		PHERR << "bad bookmark";
		return;
	}

	NSError *error = nil;
	NSData *data = [NSData dataWithBytes:bookmark.data() length:bookmark.length()];
	NSURL *url = [NSURL URLByResolvingBookmarkData:data
			options:NSURLBookmarkResolutionWithSecurityScope
			relativeToURL:nil
			bookmarkDataIsStale:nil
			error:&error];

	if(error)
		PHERR << QString::fromNSString([error localizedDescription]);
	else if(![url startAccessingSecurityScopedResource])
		PHERR << "startAccessingSecurityScopedResource errored";
}

void stopSecurityScopeBookmark(QString fileName, QByteArray bookmark)
{
	if(bookmark.length() <= 0) {
		PHERR << "bad bookmark";
		return;
	}

	NSError *error = nil;
	NSData *data = [NSData dataWithBytes:bookmark.data() length:bookmark.length()];
	NSURL *url = [NSURL URLByResolvingBookmarkData:data
			options:NSURLBookmarkResolutionWithSecurityScope
			relativeToURL:nil
			bookmarkDataIsStale:nil
			error:&error];

	if(error)
		PHERR << QString::fromNSString([error localizedDescription]);
	else
		[url stopAccessingSecurityScopedResource];
}
