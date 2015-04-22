# Script adapted from http://sleepycoders.blogspot.fr/2013/03/sharing-travis-ci-generated-files.html
exit 1 # disabled because not used anymore

# It will run only in a branch containing the word "build" to avoid repository size explosion!
if [[ "$TRAVIS_BRANCH" == *build* ]]; then
  echo -e "Starting to update gh-pages\n"

  # Just list the home folder to check the DMG is here
  ls -l $HOME/

  #go to home and setup git
  cd $HOME
  git config --global user.email "travis@travis-ci.org"
  git config --global user.name "Travis"

  #using token clone gh-pages branch
  git clone --quiet --branch=gh-pages https://${GH_TOKEN}@github.com/Phonations/Joker.git  gh-pages

  #go into directory and copy data we're interested in to that directory
  cd gh-pages
  cp -Rf $HOME/Joker_v* .

  #add, commit and push
  git add -f .
  git commit -m "Travis build $TRAVIS_BUILD_NUMBER pushed to gh-pages"
  git push -f origin gh-pages

  echo -e "Done magic\n"
fi
