!defined(VERSION, var) {
error("You must define a version number for $$TARGET")
}

win32 {
	OTHER_FILES += ../../common/common.iss

	QMAKE_POST_LINK += windeployqt $${RESOURCES_PATH} $${CS}
}

mac {
	app_bundle {
		# Info.plist
		QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string com.phonations.$$lower($${TARGET}) $${TARGET}.app/Contents/Info.plist $${CS}
		QMAKE_POST_LINK += plutil -replace CFBundleVersion -string $${BUILD} $${TARGET}.app/Contents/Info.plist $${CS}
		QMAKE_POST_LINK += plutil -replace CFBundleShortVersionString -string $${VERSION} $${TARGET}.app/Contents/Info.plist $${CS}
		QMAKE_POST_LINK += plutil -replace LSApplicationCategoryType -string public.app-category.video $${TARGET}.app/Contents/Info.plist $${CS}
		QMAKE_POST_LINK += plutil -replace LSMinimumSystemVersion -string 10.8 $${TARGET}.app/Contents/Info.plist $${CS}
		QMAKE_POST_LINK += plutil -replace NSHighResolutionCapable -string True $${TARGET}.app/Contents/Info.plist $${CS}

		# removeapp target
		removeapp.commands += rm -rf $${TARGET}.app

		QMAKE_EXTRA_TARGETS += removeapp
	}
}

CONFIG(release, debug|release) {
	PH_DEPLOY_TARGET = $${TARGET}_v$$PH_FULL_VERSION

	message($$PH_DEPLOY_LOCATION/$$PH_DEPLOY_TARGET)

	mac {
		app_bundle {
			APPLICATION_CERTIFICATE = \"3rd Party Mac Developer Application: Phonations (Y44UPVP368)\"
			INSTALLER_CERTIFICATE = \"3rd Party Mac Developer Installer: Phonations (Y44UPVP368)\"

			# Target for sandboxing
			# entitlement.plist
			OTHER_FILES += ../../common/entitlements.plist

			sandbox.commands += echo "Sandboxing $${TARGET}" $${CS}
			sandbox.commands += codesign -s $$APPLICATION_CERTIFICATE --entitlements $$TOP_ROOT/common/entitlements.plist $${TARGET}.app

			# Target for code signing
			codesign.commands += echo "Code signing $${TARGET}" $${CS}
			codesign.commands += macdeployqt $${TARGET}.app -codesign=$$APPLICATION_CERTIFICATE

			# Target for quick dmg generation
			quickdmg.commands += echo "Deploying quick dmg for $${TARGET}" $${CS}
			quickdmg.commands += macdeployqt $${TARGET}.app -dmg -verbose=2 $${CS}
			quickdmg.commands += mv $${TARGET}.dmg $${PH_DEPLOY_TARGET}.dmg $${CS}

			# Target for pretty DMG generation
			prettydmg.commands += echo "Generate $$PH_DEPLOY_TARGET:" $${CS}
			prettydmg.commands += $${_PRO_FILE_PWD_}/../../vendor/create-dmg/create-dmg \
					--volname $${PH_DEPLOY_TARGET} \
					--background $${_PRO_FILE_PWD_}/../../data/img/dmg_bg.png \
					--app-drop-link 450 218 \
					--icon $${TARGET}.app 150 218 \
					--window-size 600 450 \
					$${PH_DEPLOY_TARGET}.dmg \
					$${TARGET}.app

			# Target for ftp deployement
			ftpdeploy.commands += echo "Deploying $${PH_DEPLOY_TARGET}.dmg to $$(PH_DEPLOY_FTP_SERVER)" $${CS}
			ftpdeploy.commands += /usr/local/bin/lftp -u $$(PH_DEPLOY_FTP_USER),$$(PH_DEPLOY_FTP_PASSWORD) $$(PH_DEPLOY_FTP_SERVER) \
				 -e \"dir;put $${PH_DEPLOY_TARGET}.dmg;quit\" $${CS}

			# Target for PKG generation
			buildpkg.commands += echo "Build PKG" $${CS}
			buildpkg.commands += productbuild --component $${TARGET}.app /Applications --sign $$INSTALLER_CERTIFICATE $${PH_DEPLOY_TARGET}.pkg

			# Target for PKG and DMG cleaning
			removedmg.commands += rm $${PH_DEPLOY_TARGET}.*

			QMAKE_EXTRA_TARGETS += sandbox codesign quickdmg ftpdeploy prettydmg buildpkg removedmg
		}
	}

	win32 {
		if(exists($${_PRO_FILE_PWD_}/$$TARGET.iss)) {
			innosetup.commands += echo "Running Innosetup on $${_PRO_FILE_PWD_}/$${TARGET}.iss" $${CS}
			innosetup.commands += iscc $${_PRO_FILE_PWD_}/$${TARGET}.iss /DPWD=$$OUT_PWD $${CS}
			innosetup.commands += echo "Copy to $${PH_DEPLOY_LOCATION}" $${CS}
			innosetup.commands += copy $${TARGET}_v$${VERSION}.exe $${PH_DEPLOY_LOCATION}\\$${PH_DEPLOY_TARGET}.exe $${CS}

			QMAKE_EXTRA_TARGETS += innosetup
		}
	}
}
