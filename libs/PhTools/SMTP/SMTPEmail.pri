QT       += network

INCLUDEPATH += ../../libs/PhTools/SMPT

SOURCES += \
	../../libs/PhTools/SMTP/emailaddress.cpp \
	../../libs/PhTools/SMTP/mimeattachment.cpp \
	../../libs/PhTools/SMTP/mimefile.cpp \
	../../libs/PhTools/SMTP/mimehtml.cpp \
	../../libs/PhTools/SMTP/mimeinlinefile.cpp \
	../../libs/PhTools/SMTP/mimemessage.cpp \
	../../libs/PhTools/SMTP/mimepart.cpp \
	../../libs/PhTools/SMTP/mimetext.cpp \
	../../libs/PhTools/SMTP/smtpclient.cpp \
	../../libs/PhTools/SMTP/quotedprintable.cpp \
	../../libs/PhTools/SMTP/mimemultipart.cpp \
	../../libs/PhTools/SMTP/mimecontentformatter.cpp \
    ../../libs/PhTools/SMTP/oldmain.cpp

HEADERS  += \
	../../libs/PhTools/SMTP/emailaddress.h \
	../../libs/PhTools/SMTP/mimeattachment.h \
	../../libs/PhTools/SMTP/mimefile.h \
	../../libs/PhTools/SMTP/mimehtml.h \
	../../libs/PhTools/SMTP/mimeinlinefile.h \
	../../libs/PhTools/SMTP/mimemessage.h \
	../../libs/PhTools/SMTP/mimepart.h \
	../../libs/PhTools/SMTP/mimetext.h \
	../../libs/PhTools/SMTP/smtpclient.h \
	../../libs/PhTools/SMTP/SmtpMime \
	../../libs/PhTools/SMTP/quotedprintable.h \
	../../libs/PhTools/SMTP/mimemultipart.h \
	../../libs/PhTools/SMTP/mimecontentformatter.h
