win32 {
	QMAKE_POST_LINK += windeployqt $${RESOURCES_PATH} $${CS}
}

mac {
	app_bundle {
		QMAKE_POST_LINK += echo Create Info.plist;
		QMAKE_POST_LINK += cp $$(QTDIR)/mkspecs/macx-clang/Info.plist.app $${TARGET}.app/Contents/Info.plist;

		isEmpty(ICON) {
			message("Settings default icon")
			ICON = $$TOP_ROOT/data/icon.icns
		}

		isEmpty(BUNDLEID) {
			message("Settings default bundleid")
			BUNDLEID = com.phonations.$${TARGET}
		}

		QMAKE_POST_LINK += cp $$ICON $${TARGET}.app/Contents/Resources/$${TARGET}.icns;

		QMAKE_POST_LINK += plutil -replace CFBundleExecutable -string $${TARGET} $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -replace CFBundleIconFile -string $${TARGET}.icns $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string $${BUNDLEID} $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -replace CFBundleSignature -string ??? $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -insert CFBundleVersion -string $${VERSION} $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -insert CFBundleShortVersionString -string $${VERSION} $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -insert LSApplicationCategoryType -string public.app-category.video $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -insert NSPrincipalClass -string NSApplication $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -insert NSHighResolutionCapable -string True $${TARGET}.app/Contents/Info.plist;
	}
}

CONFIG(release, debug|release) {
	mac {
		app_bundle {
			PH_DEPLOY_TARGET = $${TARGET}_v$${VERSION}.dmg
			message($$PH_DEPLOY_TARGET)

			QMAKE_POST_LINK += macdeployqt $${TARGET}.app;

			ENTITLEMENTS = $$TOP_ROOT/common/entitlements.plist

			OTHER_FILES += $${ENTITLEMENTS} \
				$$TOP_ROOT/scripts/prepare-framework.py

##################################################
            QMAKE_POST_LINK += echo "Prepare Qt frameworks";
            QMAKE_POST_LINK += pwd;
            QMAKE_POST_LINK += $$TOP_ROOT/scripts/prepare-framework.py $(QTDIR) $${TARGET}.app;

            QMAKE_POST_LINK += echo "Update CFBundleIdentifier";
            QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtCore.framework/Resources/Info.plist;
            QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtGui.framework/Resources/Info.plist;
            QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtNetwork.framework/Resources/Info.plist;
            QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtPrintSupport.framework/Resources/Info.plist;
            QMAKE_POST_LINK += plutil -replace CFBundleExecutable -string "QtPrintSupport" $${TARGET}.app/Contents/Frameworks/QtPrintSupport.framework/Resources/Info.plist;
            QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtWidgets.framework/Resources/Info.plist;
            phgraphic {
                QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtOpenGL.framework/Resources/Info.plist;
            }
            phsony {
                QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtSerialPort.framework/Resources/Info.plist;
            }
            QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtSql.framework/Resources/Info.plist;
            QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/QtXml.framework/Resources/Info.plist;

            QMAKE_POST_LINK += echo "Sign Qt frameworks with $$(APPLICATION_CERTIFICATE)";

            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtCore.framework;
            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtGui.framework;
            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtNetwork.framework;
            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtPrintSupport.framework;
            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtWidgets.framework;
            phgraphic {
                QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtOpenGL.framework;
            }
            phsony {
                QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtSerialPort.framework;
            }
            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtSql.framework;
            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/QtXml.framework;

            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/PlugIns/*/*.dylib;
            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep $${TARGET}.app/Contents/Frameworks/*.*.*;

            QMAKE_POST_LINK += echo "Add entitlements";

            QMAKE_POST_LINK += cp $${ENTITLEMENTS} .;
            QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -v --entitlements $${ENTITLEMENTS} $${TARGET}.app;

            QMAKE_POST_LINK += echo "Build PKG";

            QMAKE_POST_LINK += echo $$(INSTALLER_CERTIFICATE);
			QMAKE_POST_LINK += productbuild --component $${TARGET}.app /Applications --sign $$(INSTALLER_CERTIFICATE) $${TARGET}_v$${VERSION}.pkg;

##################################################

			# creating dmg with create-dmg

			QMAKE_POST_LINK += $${_PRO_FILE_PWD_}/../../vendor/create-dmg/create-dmg \
				--volname $${TARGET}_v$${VERSION} \
				--background $${_PRO_FILE_PWD_}/../../data/img/dmg_bg.png \
				--app-drop-link 450 218 \
				--icon $${TARGET}.app 150 218 \
				--window-size 600 450 \
				$${PH_DEPLOY_TARGET} \
				$${TARGET}.app;

			QMAKE_POST_LINK += cp $${PH_DEPLOY_TARGET} $${PH_DEPLOY_LOCATION};
		}
	}

	win32 {
		QMAKE_POST_LINK += echo "Deploying Joker" $${CS}
		QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/app/Joker/JokerSetup.iss) . $${CS}
		QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/LICENSE.TXT) . $${CS}
		QMAKE_POST_LINK += ISCC.exe JokerSetup.iss $${CS}
	}
}
