#include "PhHelpMenu.h"

#include <QApplication>
#import <AppKit/NSApplication.h>

PhHelpMenu::PhHelpMenu(QWidget *parent) : QMenu(parent)
{
	NSApplication *app = [NSApplication sharedApplication];
	app.helpMenu = this->toNSMenu();
}
