current_tag=$(git describe --exact-match HEAD 2>&1)
if [ "$?" -ne 0 ]; then
  echo "Not on tagged commit"
  # use previous tag on branch
  export prev_tag=$(git describe --tags --abbrev=0 origin/release)
else
  echo "On tagged commit $current_tag"
  # this is the case if we are on the release branch
  # use tag before this tag
  export prev_tag=$(git describe --tags --abbrev=0 origin/release~)
fi

echo "Previous tag to build first $prev_tag"
