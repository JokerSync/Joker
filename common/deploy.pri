!defined(VERSION, var) {
error("You must define a version number for $$TARGET")
}

win32 {
	OTHER_FILES += ../../common/common.iss

	QMAKE_POST_LINK += windeployqt $${RESOURCES_PATH} $${CS}
}

mac {
	app_bundle {
		QMAKE_POST_LINK += plutil -replace CFBundleVersion -string $${VERSION} $${TARGET}.app/Contents/Info.plist;
		QMAKE_POST_LINK += plutil -replace NSHighResolutionCapable -string True $${TARGET}.app/Contents/Info.plist;

		removeapp.commands += rm -rf $${TARGET}.app

		QMAKE_EXTRA_TARGETS += removeapp
	}
}

CONFIG(release, debug|release) {
	if(equals(PH_GIT_BRANCH, "master") || equals(PH_GIT_BRANCH, "HEAD")) {
		PH_DEPLOY_TARGET = $${TARGET}_v$${VERSION}
	} else {
		PH_DEPLOY_TARGET = $${TARGET}_v$${VERSION}_$${PH_GIT_BRANCH}
	}
	message($$PH_DEPLOY_TARGET)

	mac {
		app_bundle {
			QMAKE_POST_LINK += macdeployqt $${TARGET}.app; # -codesign=$$(APPLICATION_CERTIFICATE);


			installer.commands += echo "Generate $$PH_DEPLOY_TARGET:";
			installer.commands += $${_PRO_FILE_PWD_}/../../vendor/create-dmg/create-dmg \
					--volname $${PH_DEPLOY_TARGET} \
#					--volicon $${_PRO_FILE_PWD_}/../../app/Joker/joker.icns \
					--background $${_PRO_FILE_PWD_}/../../data/img/dmg_bg.png \
					--app-drop-link 450 218 \
					--icon $${TARGET}.app 150 218 \
					--window-size 600 450 \
					$${PH_DEPLOY_TARGET}.dmg \
					$${TARGET}.app &&
			installer.commands += echo Copying to $${PH_DEPLOY_LOCATION} &&
			installer.commands += cp $${PH_DEPLOY_TARGET}.dmg $${PH_DEPLOY_LOCATION} &&
			installer.commands += open -R $${PH_DEPLOY_LOCATION}/$${PH_DEPLOY_TARGET}.dmg

			removedmg.commands += rm $${PH_DEPLOY_TARGET}.dmg

			QMAKE_EXTRA_TARGETS += removedmg
		}
	}

	win32 {
		message($$PH_DEPLOY_LOCATION)
		if(exists($${_PRO_FILE_PWD_}/$$TARGET.iss)) {
			installer.commands += echo "Running Innosetup on $${_PRO_FILE_PWD_}/$${TARGET}.iss" &
			installer.commands += iscc $${_PRO_FILE_PWD_}/$${TARGET}.iss /DPWD=$$OUT_PWD &
			installer.commands += echo "Copy to $${PH_DEPLOY_LOCATION}" &
			installer.commands += copy $${TARGET}_v$${VERSION}.exe $${PH_DEPLOY_LOCATION}\\$${PH_DEPLOY_TARGET}.exe
		}
	}
}

QMAKE_EXTRA_TARGETS += installer
