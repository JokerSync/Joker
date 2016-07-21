win32 {
	QMAKE_POST_LINK += windeployqt --qmldir $$shell_path($$_PRO_FILE_PWD_) $$shell_path($${RESOURCES_PATH}/Joker.exe) -multimediaquick $${CS}
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

			QMAKE_POST_LINK += $${_PRO_FILE_PWD_}/../../vendor/create-dmg/create-dmg \
				--volname $${PH_DEPLOY_TARGET} \
				--volicon $${_PRO_FILE_PWD_}/../../app/Joker/joker.icns \
				--background $${_PRO_FILE_PWD_}/../../data/img/dmg_bg.png \
				--app-drop-link 450 218 \
				--icon $${TARGET}.app 150 218 \
				--window-size 600 450 \
				$${PH_DEPLOY_TARGET}.dmg \
				$${TARGET}.app;

			QMAKE_POST_LINK += cp $${PH_DEPLOY_TARGET}.dmg $${PH_DEPLOY_LOCATION};
		}
	}

	win32 {
		if(exists($${_PRO_FILE_PWD_}/$$TARGET.iss)) {
			message("Processing $$TARGET")
			QMAKE_POST_LINK += echo "Deploying $${_PRO_FILE_PWD_}/$${TARGET}.iss" $${CS}
			QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/$${TARGET}.iss) . $${CS}
			QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/LICENSE.TXT) . $${CS}
			QMAKE_POST_LINK += echo $$(PATH) $${CS}
			QMAKE_POST_LINK += iscc "$${TARGET}.iss" $${CS}
			QMAKE_POST_LINK += dir

		}
	}
}
