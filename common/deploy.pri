win32 {
	QMAKE_POST_LINK += windeployqt $${RESOURCES_PATH} $${CS}
}

mac {
	app_bundle {

		BUNDLEID = com.phonations.$$lower($${TARGET})

		QMAKE_POST_LINK += cp $$ICON $${TARGET}.app/Contents/Resources/$${TARGET}.icns;

		QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string $${BUNDLEID} $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -insert CFBundleVersion -string $${VERSION} $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -insert CFBundleShortVersionString -string $${VERSION} $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -insert LSApplicationCategoryType -string public.app-category.video $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -insert NSHighResolutionCapable -string True $${TARGET}.app/Contents/Info.plist;
	}
}

CONFIG(release, debug|release) {
	mac {
		app_bundle {
			if(equals(PH_GIT_BRANCH, "master") || equals(PH_GIT_BRANCH, "HEAD")) {
				PH_DEPLOY_TARGET = $${TARGET}_v$${VERSION}
			} else {
				PH_DEPLOY_TARGET = $${TARGET}_v$${VERSION}_$${PH_GIT_BRANCH}
			}

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

			create-pkg {
				QMAKE_POST_LINK += echo "Build PKG";

				QMAKE_POST_LINK += echo $$(INSTALLER_CERTIFICATE);
				QMAKE_POST_LINK += productbuild --component $${TARGET}.app /Applications --sign $$(INSTALLER_CERTIFICATE) $${PH_DEPLOY_TARGET}.pkg;

				QMAKE_POST_LINK += cp $${PH_DEPLOY_TARGET}.pkg $${PH_DEPLOY_LOCATION};
			}

##################################################

			installer.commands += echo "Creating dmg with create-dmg";
			installer.commands += $${_PRO_FILE_PWD_}/../../vendor/create-dmg/create-dmg \
					--volname $${PH_DEPLOY_TARGET} \
#					--volicon $${_PRO_FILE_PWD_}/../../app/Joker/joker.icns \
					--background $${_PRO_FILE_PWD_}/../../data/img/dmg_bg.png \
					--app-drop-link 450 218 \
					--icon $${TARGET}.app 150 218 \
					--window-size 600 450 \
					$${PH_DEPLOY_TARGET}.dmg \
					$${TARGET}.app &&
			installer.commands += cp $${PH_DEPLOY_TARGET}.dmg $${PH_DEPLOY_LOCATION}
		}
	}

	win32 {
		if(exists($${_PRO_FILE_PWD_}/$$TARGET.iss)) {
			installer.commands += echo "Running Innosetup on $${_PRO_FILE_PWD_}/$${TARGET}.iss" &
			installer.commands += iscc $${_PRO_FILE_PWD_}/$${TARGET}.iss /DPWD=$$OUT_PWD
		}
	}
}

QMAKE_EXTRA_TARGETS += installer
