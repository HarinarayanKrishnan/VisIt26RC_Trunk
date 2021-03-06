function bv_hdf5_initialize
{
export DO_HDF5="no"
export ON_HDF5="off"
export USE_SYSTEM_HDF5="no"
add_extra_commandline_args "hdf5" "alt-hdf5-dir" 1 "Use alternative directory for hdf5"
}

function bv_hdf5_enable
{
DO_HDF5="yes"
ON_HDF5="on"
}

function bv_hdf5_disable
{
DO_HDF5="no"
ON_HDF5="off"
}

function bv_hdf5_alt_hdf5_dir
{
    bv_hdf5_enable
    USE_SYSTEM_HDF5="yes"
    HDF5_INSTALL_DIR="$1"
}

function bv_hdf5_depends_on
{
    if [[ "$USE_SYSTEM_HDF5" == "yes" ]]; then
        echo ""
    else
        echo "szip"
    fi
}

function bv_hdf5_initialize_vars
{
    if [[ "$USE_SYSTEM_HDF5" == "no" ]]; then
        HDF5_INSTALL_DIR="${VISITDIR}/hdf5/$HDF5_VERSION/${VISITARCH}"
    fi
}

function bv_hdf5_info
{
export HDF5_VERSION=${HDF5_VERSION:-"1.8.7"}
export HDF5_FILE=${HDF5_FILE:-"hdf5-${HDF5_VERSION}.tar.gz"}
export HDF5_COMPATIBILITY_VERSION=${HDF5_COMPATIBILITY_VERSION:-"1.8"}
export HDF5_BUILD_DIR=${HDF5_BUILD_DIR:-"hdf5-${HDF5_VERSION}"}
# Note: Versions of HDF5 1.6.5 and earlier DO NOT have last path component
export HDF5_URL=${HDF5_URL:-"http://www.hdfgroup.org/ftp/HDF5/prev-releases/hdf5-${HDF5_VERSION}/src"}
export HDF5_MD5_CHECKSUM="37711d4bcb72997e93d495f97c76c33a"
export HDF5_SHA256_CHECKSUM=""
}

function bv_hdf5_print
{
  printf "%s%s\n" "HDF5_FILE=" "${HDF5_FILE}"
  printf "%s%s\n" "HDF5_VERSION=" "${HDF5_VERSION}"
  printf "%s%s\n" "HDF5_COMPATIBILITY_VERSION=" "${HDF5_COMPATIBILITY_VERSION}"
  printf "%s%s\n" "HDF5_BUILD_DIR=" "${HDF5_BUILD_DIR}"
}

function bv_hdf5_print_usage
{
printf "%-15s %s [%s]\n" "--hdf5" "Build HDF5" "${DO_HDF5}"
}

function bv_hdf5_graphical
{
local graphical_out="HDF5     $HDF5_VERSION($HDF5_FILE)      $ON_HDF5"
echo $graphical_out
}

function bv_hdf5_host_profile
{
    if [[ "$DO_HDF5" == "yes" ]] ; then
        echo >> $HOSTCONF
        echo "##" >> $HOSTCONF
        echo "## HDF5" >> $HOSTCONF
        echo "##" >> $HOSTCONF

        if [[ "$USE_SYSTEM_HDF5" == "yes" ]]; then
            echo \
            "VISIT_OPTION_DEFAULT(VISIT_HDF5_DIR $HDF5_INSTALL_DIR)" \
            >> $HOSTCONF 
        else
            echo \
            "VISIT_OPTION_DEFAULT(VISIT_HDF5_DIR \${VISITHOME}/hdf5/$HDF5_VERSION/\${VISITARCH})" \
            >> $HOSTCONF 
            if [[ "$DO_SZIP" == "yes" ]] ; then
                echo \
                "VISIT_OPTION_DEFAULT(VISIT_HDF5_LIBDEP \${VISITHOME}/szip/$SZIP_VERSION/\${VISITARCH}/lib sz /usr/lib z TYPE STRING)" \
                >> $HOSTCONF
            else
                echo \
                "VISIT_OPTION_DEFAULT(VISIT_HDF5_LIBDEP /usr/lib z TYPE STRING)" \
                >> $HOSTCONF
            fi
        fi
    fi
}

function bv_hdf5_ensure
{
    if [[ "$DO_HDF5" == "yes" && "$USE_SYSTEM_HDF5" == "no" ]] ; then
        ensure_built_or_ready "hdf5" $HDF5_VERSION $HDF5_BUILD_DIR $HDF5_FILE $HDF5_URL 
        if [[ $? != 0 ]] ; then
            ANY_ERRORS="yes"
            DO_HDF5="no"
            error "Unable to build HDF5.  ${HDF5_FILE} not found."
        fi
    fi
}

function bv_hdf5_dry_run
{
  if [[ "$DO_HDF5" == "yes" ]] ; then
    echo "Dry run option not set for hdf5."
  fi
}

# *************************************************************************** #
#                          Function 8.1, build_hdf5                           #
# *************************************************************************** #

function build_hdf5
{
    #
    # Prepare build dir
    #
    prepare_build_dir $HDF5_BUILD_DIR $HDF5_FILE
    untarred_hdf5=$?
    if [[ $untarred_hdf5 == -1 ]] ; then
       warn "Unable to prepare HDF5 Build Directory. Giving Up"
       return 1
    fi

    #
    info "Configuring HDF5 . . ."
    cd $HDF5_BUILD_DIR || error "Can't cd to HDF5 build dir."
    cf_darwin=""
    if [[ "$OPSYS" == "Darwin" ]]; then
        export DYLD_LIBRARY_PATH="$VISITDIR/szip/$SZIP_VERSION/$VISITARCH/lib":$DYLD_LIBRARY_PATH
    else
        export LD_LIBRARY_PATH="$VISITDIR/szip/$SZIP_VERSION/$VISITARCH/lib":$LD_LIBRARY_PATH
    fi
    if [[ "$DO_STATIC_BUILD" == "yes" ]]; then
            cf_build_type="--disable-shared --enable-static"
        else
            cf_build_type="--enable-shared --disable-static"
    fi
    cf_szip=""
    if test "x${DO_SZIP}" = "xyes"; then
        info "SZip requested.  Configuring HDF5 with SZip support."
        sz_dir="${VISITDIR}/szip/${SZIP_VERSION}/${VISITARCH}"
        cf_szip="--with-szlib=${sz_dir}"
    fi

    if [[ "$FC_COMPILER" == "no" ]] ; then
        FORTRANARGS=""
    else
        FORTRANARGS="FC=\"$FC_COMPILER\" F77=\"$FC_COMPILER\" FCFLAGS=\"$FCFLAGS\" FFLAGS=\"$FCFLAGS\" --enable-fortran"
    fi

    # In order to ensure $FORTRANARGS is expanded to build the arguments to
    # configure, we wrap the invokation in 'sh -c "..."' syntax
    info "Invoking command to configure HDF5"
    info "./configure CXX=\"$CXX_COMPILER\" CC=\"$C_COMPILER\" \
        CFLAGS=\"$CFLAGS $C_OPT_FLAGS\" CXXFLAGS=\"$CXXFLAGS $CXX_OPT_FLAGS\" \
        $FORTRANARGS \
        --prefix=\"$VISITDIR/hdf5/$HDF5_VERSION/$VISITARCH\" \
        ${cf_szip} ${cf_darwin}"
    sh -c "./configure CXX=\"$CXX_COMPILER\" CC=\"$C_COMPILER\" \
        CFLAGS=\"$CFLAGS $C_OPT_FLAGS\" CXXFLAGS=\"$CXXFLAGS $CXX_OPT_FLAGS\" \
        $FORTRANARGS \
        --prefix=\"$VISITDIR/hdf5/$HDF5_VERSION/$VISITARCH\" \
        ${cf_szip} ${cf_build_type}"
    if [[ $? != 0 ]] ; then
       warn "HDF5 configure failed.  Giving up"
       return 1
    fi

    #
    # Build HDF5
    #
    info "Making HDF5 . . ."
    $MAKE $MAKE_OPT_FLAGS
    if [[ $? != 0 ]] ; then
       warn "HDF5 build failed.  Giving up"
       return 1
    fi
    #
    # Install into the VisIt third party location.
    #
    info "Installing HDF5 . . ."

    $MAKE install
    if [[ $? != 0 ]] ; then
       warn "HDF5 install failed.  Giving up"
       return 1
    fi

    if [[ "$DO_STATIC_BUILD" == "no" && "$OPSYS" == "Darwin" ]]; then
        #
        # Make dynamic executable, need to patch up the install path and
        # version information.
        #
        info "Creating dynamic libraries for HDF5 . . ."
    fi

    if [[ "$DO_GROUP" == "yes" ]] ; then
       chmod -R ug+w,a+rX "$VISITDIR/hdf5"
       chgrp -R ${GROUP} "$VISITDIR/hdf5"
    fi
    cd "$START_DIR"
    info "Done with HDF5"
    return 0
}

function bv_hdf5_is_enabled
{
    if [[ $DO_HDF5 == "yes" ]]; then
        return 1    
    fi
    return 0
}

function bv_hdf5_is_installed
{

    if [[ "$USE_SYSTEM_HDF5" == "yes" ]]; then
        return 1
    fi

    check_if_installed "hdf5" $HDF5_VERSION
    if [[ $? == 0 ]] ; then
        return 1
    fi
    return 0
}

function bv_hdf5_build
{
cd "$START_DIR"

if [[ "$DO_HDF5" == "yes" && "$USE_SYSTEM_HDF5" == "no" ]] ; then
    check_if_installed "hdf5" $HDF5_VERSION
    if [[ $? == 0 ]] ; then
        info "Skipping HDF5 build.  HDF5 is already installed."
    else
        info "Building HDF5 (~15 minutes)"
        build_hdf5
        if [[ $? != 0 ]] ; then
            error "Unable to build or install HDF5.  Bailing out."
        fi
        info "Done building HDF5"
    fi
fi
}
