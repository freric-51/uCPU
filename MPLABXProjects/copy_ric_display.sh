#!/usr/bin/env bash

function exe_backup {
	#	echo -e "-$1-"
	#	echo -e "-$2-"
	cp --verbose --preserve --update "$1"/*.* "$2"
	cp --verbose --preserve --update "$2"/*.* "$1"
}

echo -e "----------------------------\nInicio... verifique manualmente arquivos que devem ser apagados!\n----------------------------"
# =========================================
A="/home/ric/Documentos/code/MPLABXProjects"
B="/home/ric/Documentos/code/GitHub/uCPU/MPLABXProjects"
# =========================================

# -------------------------------------------------------------
C=""
exe_backup "$A$C" "$B$C"

# -------------------------------------------------------------
C="/ric_display_pic.X"
exe_backup "$A$C" "$B$C"

# -------------------------------------------------------------
C="/ric_display_pic.X/doxygen"
# 1st remove unwanted files
cd $A$C
$A$C/clear_latex.sh
cd $A

exe_backup "$A$C" "$B$C"

# -------------------------------------------------------------
C="/ric_display_pic.X/doxygen/Imagens"
exe_backup "$A$C" "$B$C"

# -------------------------------------------------------------
C="/ric_display_pic.X/doxygen/latex"
exe_backup "$A$C" "$B$C"

# -------------------------------------------------------------
C="/ric_display_pic.X/doxygen/logo"
exe_backup "$A$C" "$B$C"

# -------------------------------------------------------------
C="/ric_display_pic.X/inc"
exe_backup "$A$C" "$B$C"

# -------------------------------------------------------------
C="/ric_display_pic.X/nbproject"
exe_backup "$A$C" "$B$C"

# -------------------------------------------------------------
C="/ric_display_pic.X/nbproject/private"
exe_backup "$A$C" "$B$C"


# -------------------------------------------------------------
echo -e "Fim..."
sleep 15

