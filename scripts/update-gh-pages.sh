if [ "$TRAVIS_PULL_REQUEST" == "false" ]; then
  echo -e "Starting to update gh-pages\n"

  # Just list the home folder to check the DMG is here
  ls -l $HOME/

  #go to home and setup git
  cd $HOME
  git config --global user.email "travis@travis-ci.org"
  git config --global user.name "Travis"

  #using token clone gh-pages branch
  git clone --quiet --branch=gh-pages https://${GH_TOKEN}@github.com/Phonations/TravisTest.git  gh-pages

  #go into directory and copy data we're interested in to that directory
  cd gh-pages
  cp -Rf $HOME/Joker_v* .

  echo "add"
  git add -f .
  echo "commit"
  git commit -m "Travis build $TRAVIS_BUILD_NUMBER pushed to gh-pages"
  echo "push"
  git push -f origin gh-pages

  echo -e "Done magic\n"
fi
