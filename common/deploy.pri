win32 {
	QMAKE_POST_LINK += $$(QTDIR)/bin/windeployqt $${RESOURCES_PATH} $${CS}
}

CONFIG(release, debug|release) {
	mac {
		app_bundle {

			PH_DEPLOY_TARGET = $${PH_DEPLOY_LOCATION}/$${TARGET}_v$${VERSION}.dmg
			message($$PH_DEPLOY_TARGET)

			!exists(/usr/local/bin/appdmg) {
					error("You must install appdmg : https://github.com/LinusU/node-appdmg")
			}

			QMAKE_PRE_LINK += rm -f $${TARGET}.app/Contents/Resources/qt.conf;

			QMAKE_POST_LINK += macdeployqt $${TARGET}.app;

			ENTITLEMENTS = $$TOP_ROOT/app/Joker/entitlements.plist

			OTHER_FILES += $${ENTITLEMENTS}

##################################################
			BUNDLEID += com.phonations.joker

			QMAKE_POST_LINK += cp $(QTDIR)/lib/QtCore.framework/Contents/Info.plist $${TARGET}.app/Contents/Frameworks/QtCore.framework/Resources;
			QMAKE_POST_LINK += cp $(QTDIR)/lib/QtGui.framework/Contents/Info.plist $${TARGET}.app/Contents/Frameworks/QtGui.framework/Resources;
			QMAKE_POST_LINK += cp $(QTDIR)/lib/QtNetwork.framework/Contents/Info.plist $${TARGET}.app/Contents/Frameworks/QtNetwork.framework/Resources;
			QMAKE_POST_LINK += cp $(QTDIR)/lib/QtPrintSupport.framework/Contents/Info.plist $${TARGET}.app/Contents/Frameworks/QtPrintSupport.framework/Resources;
			QMAKE_POST_LINK += cp $(QTDIR)/lib/QtWidgets.framework/Contents/Info.plist $${TARGET}.app/Contents/Frameworks/QtWidgets.framework/Resources;
			QMAKE_POST_LINK += cp $(QTDIR)/lib/QtOpenGL.framework/Contents/Info.plist $${TARGET}.app/Contents/Frameworks/QtOpenGL.framework/Resources;
			QMAKE_POST_LINK += cp $(QTDIR)/lib/QtSerialPort.framework/Contents/Info.plist $${TARGET}.app/Contents/Frameworks/QtSerialPort.framework/Resources;
			QMAKE_POST_LINK += cp $(QTDIR)/lib/QtSql.framework/Contents/Info.plist $${TARGET}.app/Contents/Frameworks/QtSql.framework/Resources;
			QMAKE_POST_LINK += cp $(QTDIR)/lib/QtXml.framework/Contents/Info.plist $${TARGET}.app/Contents/Frameworks/QtXml.framework/Resources;

			QMAKE_POST_LINK += echo "coucou";
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/*.*;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/QtCore.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/QtGui.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/QtNetwork.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/QtPrintSupport.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/QtWidgets.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/QtOpenGL.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/QtSerialPort.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/QtSql.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/QtXml.framework;

			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/PlugIns/*/*.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/accessible/*.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/imageformats/*.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/platforms/*.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/printsupport/*.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/PlugIns/
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/*.dylib;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID $${TARGET}.app/Contents/Frameworks/*.*.*;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libavcodec.55.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libavformat.55.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libavutil.52.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libfaac.0.0.0.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libltc.11.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libmp3lame.0.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libportaudio.2.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libswresample.0.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libswscale.2.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/libx264.142.dylib;
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/
#			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) $${TARGET}.app/Contents/Frameworks/
			QMAKE_POST_LINK += echo "sign all";

			QMAKE_POST_LINK += cp $${ENTITLEMENTS} .;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -v --entitlements $${ENTITLEMENTS} $${TARGET}.app;

#			QMAKE_POST_LINK += productbuild --component $${TARGET}.app /Applications -sign $$(INSTALLER_CERTIFICATE) $${TARGET}.pkg;
			QMAKE_POST_LINK += productbuild --component $${TARGET}.app /Applications $${TARGET}.pkg;

##################################################


#			QMAKE_POST_LINK += export PATH=/usr/local/bin:$$(PATH);
#			QMAKE_POST_LINK += cp $${_PRO_FILE_PWD_}/../../data/img/dmg_bg.png .;
#			QMAKE_POST_LINK += sed -e "s/@TARGET@/$${TARGET}/g" $${_PRO_FILE_PWD_}/../../common/appdmg.json > appdmg.json;
#			QMAKE_POST_LINK += rm $${PH_DEPLOY_TARGET};
#			QMAKE_POST_LINK += appdmg appdmg.json $${PH_DEPLOY_TARGET};
		}
	}

	win32 {
		QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/app/Joker/JokerSetup.iss) . $${CS}
		QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/LICENSE.TXT) . $${CS}
		QMAKE_POST_LINK += iscc JokerSetup.iss $${CS}
	}
}

OTHER_FILES += \
	../../common/appdmg.json
