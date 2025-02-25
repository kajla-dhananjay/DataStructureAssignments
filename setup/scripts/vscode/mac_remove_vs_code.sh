#!/bin/bash

echo "Uninstalling all VS Code extensions..."

extensions=$(code --list-extensions)
for ext in $extensions; do
  echo "Removing extension: $ext..."
  code --uninstall-extension $ext
done

echo "Uninstalling Visual Studio Code..."
brew uninstall --cask visual-studio-code

echo "Removing leftover VS Code configuration files..."
rm -rf ~/Library/Application\ Support/Code
rm -rf ~/.vscode
rm -rf ~/Library/Caches/com.microsoft.VSCode
rm -rf ~/Library/Preferences/com.microsoft.VSCode.plist
rm -rf ~/Library/Saved\ Application\ State/com.microsoft.VSCode.savedState
rm -rf ~/.vscode-remote

echo "All VS Code extensions and data have been removed successfully!"

