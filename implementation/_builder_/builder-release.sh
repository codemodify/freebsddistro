
###########################################################################################################
###########################################################################################################
###########################################################################################################

# path to Qt's bin directory
qtBinPath="/home/nicu/Applications/Frameworks/Qt/4.6.2/release-shared/bin"


# path where to install the apps
installPath="/home/nicu/Applications/abcdefghi-release"

###########################################################################################################
###########################################################################################################
###########################################################################################################
































































































































# helpers
##########################################################################
targetFolder=""

deleteAllMakefiles()
{
    if [ "$targetFolder" != "" ]; then

        # removing the 'Makefile' from all subdirs
        makeFiles=`find $targetFolder | grep Makefile`
        for makeFile in ${makeFiles} ; do
            echo "removing:" $makeFile
            rm $makeFile
        done
    fi
}


# build targets
##########################################################################
build()
{
    clear
    echo
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo "// building"
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo
            "$qtBinPath/qmake" -makefile builder.pro
            make all
    echo
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo "// done building"
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo
    #read stdin
}

clean()
{
    clear
    echo
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo "// cleaning compiled files"
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo
            make clean
    echo
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo "// done cleaning"
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo
    #read stdin

    clear
    echo
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo "// removing any temporary files"
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo
            targetFolder="../blocks"
            deleteAllMakefiles

            targetFolder="../applications"
            deleteAllMakefiles

            targetFolder="../server"
            deleteAllMakefiles

            echo "removing:" `pwd`/../out
            rm -r `pwd`/../out
    echo
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo "// done removing"
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo
    #read stdin
}

install()
{
    cp -d `pwd`/../out/bin/* $installPath/

    targetFolder="../applications"
    cp -d -r $targetFolder/defaults/* $installPath/

    targetFolder="../server"
    cp -d -r $targetFolder/defaults/* $installPath/
}

backup()
{
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo "// Starting Backup"
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo

	currentDate=`date --rfc-333=date`

	cd ../../..
	tar -czvf SoftwareDistribution-backup-$currentDate.tar.gz SoftwareDistribution

    echo
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo "// Backup Done"
    echo "// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----"
    echo
}


# compilling
# 'build' / 'clean' / 'rebuild' / 'install'
##########################################################################
case $1 in
    'build')
        build
        ;;

    'clean')
        clean
        ;;

    'rebuild')
        clean
        build
        ;;

    'install')
        install
        ;;

	'backup')
		backup
		;;

    *)
        clear
        echo "no action specified."
        echo "options: build / clean / rebuild / install"
        ;;
esac
