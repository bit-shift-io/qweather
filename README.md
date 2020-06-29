# qweather
Quick Weather QML Qt application for desktop and mobile. Using BOM weather for Australia.

# requirements
Install qt5-charts && qt5-graphicaleffects  
Install gdb   

# android
https://wiki.qt.io/Android

Install jdk8 or jdk8-openjdk (preferred)  
JDK directory: /usr/lib/jvm/default

Enable adb run command: adb devices  

Install gradle  

Install qt from here, dont use the aur version  
wget http://download.qt.io/official_releases/qt/5.12/5.12.9/qt-opensource-linux-x64-5.12.9.run

When installing android tools, will warn no space on device, exec the following:  
sudo mount -o remount,size=8G /tmp/



# old android notes
pip install aqtinstaller
aqt install 5.11.3 linux ....


QT Creator > Tools > Settings > Devices > Android  
The following will work from qt creator gui installing jdk8 (no newer version supported), these instructions are a backup method...  
https://doc.qt.io/qtcreator/creator-developing-android.html  

Command line tools from sdk page /android/android-sdk/cmdline-tools/tools/  
https://developer.android.com/studio  
run from the tools folder:  
./sdkmanager --update  
./sdkmanager "platform-tools" "platforms;android-29" "build-tools;29.0.2" "ndk;21.1.6352462"    
./avdmanager create avd -c 100M -k "system-images;android-30;google_apis;x86" -n Nexus_6P_API_30 -d "Nexus 6P"  

Add path to ~/.bashrc  
export ANDROID_SDK_ROOT=$HOME/android
export PATH=$PATH:$ANDROID_SDK_ROOT/cmdline-tools/tools/bin
export PATH=$PATH:$ANDROID_SDK_ROOT/platform-tools
export PATH=$PATH:$ANDROID_SDK_ROOT/ndk-bundle