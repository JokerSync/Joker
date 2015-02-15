win32 {
	QMAKE_POST_LINK += windeployqt $${RESOURCES_PATH} $${CS}
}

CONFIG(release, debug|release) {
	mac {
		app_bundle {
			PH_DEPLOY_TARGET = $${TARGET}_v$${VERSION}.dmg
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

			QMAKE_POST_LINK += echo "Prepare Qt frameworks";
			QMAKE_POST_LINK += $$TOP_ROOT/scripts/prepare-framework.py $(QTDIR) $${TARGET}.app;

			QMAKE_POST_LINK += echo "Update CFBundleIdentifier";
			QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtCore.framework/Resources/Info.plist;
			QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtGui.framework/Resources/Info.plist;
			QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtNetwork.framework/Resources/Info.plist;
			QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtPrintSupport.framework/Resources/Info.plist;
			QMAKE_POST_LINK += plutil -replace CFBundleExecutable -string "QtPrintSupport" $${TARGET}.app/Contents/Frameworks/QtPrintSupport.framework/Resources/Info.plist;
			QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtWidgets.framework/Resources/Info.plist;
			QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtOpenGL.framework/Resources/Info.plist;
			QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtSerialPort.framework/Resources/Info.plist;
			QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtSql.framework/Resources/Info.plist;
			QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtXml.framework/Resources/Info.plist;

			QMAKE_POST_LINK += echo "Sign Qt frameworks";

			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtCore.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtGui.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtNetwork.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtPrintSupport.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtWidgets.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtOpenGL.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtSerialPort.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtSql.framework;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtXml.framework;

			QMAKE_POST_LINK += echo "Sign SDL frameworks";

			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/SDL2.framework/Versions/A --force;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/webp.framework/Versions/A --force --verbose;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/SDL2_image.framework/Versions/A --force --verbose;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/FreeType.framework/Versions/A --force --verbose;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/SDL2_ttf.framework/Versions/A --force --verbose;

			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/PlugIns/*/*.dylib;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/*.*.*;

			QMAKE_POST_LINK += echo "Add entitlements";

			QMAKE_POST_LINK += cp $${ENTITLEMENTS} .;
			QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -v --entitlements $${ENTITLEMENTS} $${TARGET}.app;

			QMAKE_POST_LINK += echo "Build PKG";

			QMAKE_POST_LINK += echo $$(INSTALLER_CERTIFICATE);
			QMAKE_POST_LINK += productbuild --component $${TARGET}.app /Applications --sign $$(INSTALLER_CERTIFICATE) $${TARGET}.pkg;

##################################################


#			QMAKE_POST_LINK += export PATH=/usr/local/bin:$$(PATH);
#			QMAKE_POST_LINK += cp $${_PRO_FILE_PWD_}/../../data/img/dmg_bg.png .;
#			QMAKE_POST_LINK += sed -e "s/@TARGET@/$${TARGET}/g" $${_PRO_FILE_PWD_}/../../common/appdmg.json > appdmg.json;
#			QMAKE_POST_LINK += rm $${PH_DEPLOY_TARGET};
#			QMAKE_POST_LINK += appdmg appdmg.json $${PH_DEPLOY_TARGET};
#			QMAKE_POST_LINK += cp $${PH_DEPLOY_TARGET} $${PH_DEPLOY_LOCATION};
		}
	}

	win32 {
		QMAKE_POST_LINK += echo "Deploying Joker" $${CS}
		QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/app/Joker/JokerSetup.iss) . $${CS}
		QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/LICENSE.TXT) . $${CS}
		QMAKE_POST_LINK += ISCC.exe JokerSetup.iss $${CS}
	}
}

OTHER_FILES += \
	../../common/appdmg.json
