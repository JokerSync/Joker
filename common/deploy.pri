win32 {
	QMAKE_POST_LINK += windeployqt $${RESOURCES_PATH} $${CS}
}

CONFIG(release, debug|release) {
	mac {
		app_bundle {
			PH_DEPLOY_TARGET = $${TARGET}_v$${VERSION}.dmg
			message($$PH_DEPLOY_TARGET)

			QMAKE_POST_LINK += macdeployqt $${TARGET}.app;

			QMAKE_POST_LINK += $${_PRO_FILE_PWD_}/../../vendor/create-dmg/create-dmg \
				--volname $${TARGET}_v$${VERSION} \
				--volicon $${_PRO_FILE_PWD_}/../../app/Joker/joker.icns \
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
