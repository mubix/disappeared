# Copyright 1999-2006 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: /var/cvsroot/gentoo-x86/net-www/mod_security/mod_security-1.9.4.ebuild,v 1.1 2006/06/05 18:50:20 hollow Exp $

inherit eutils apache-module

MY_P="${P/mod_security-/modsecurity-apache_}"
S="${WORKDIR}/${MY_P}"

DESCRIPTION="Intrusion Detection System for Apache."
HOMEPAGE="http://www.modsecurity.org/"
SRC_URI="http://www.modsecurity.org/download/${MY_P}.tar.gz"
LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~amd64 ~mips ~ppc ~sparc ~x86"
IUSE="doc"

APXS1_ARGS="-S LIBEXECDIR=${S} -c ${S}/apache1/mod_security2.c"
APACHE1_MOD_FILE="apache1/${PN}2.so"
APACHE1_MOD_CONF="1.8.6/99_mod_security"
APACHE1_MOD_DEFINE="SECURITY"

APXS2_ARGS="-S LIBEXECDIR=${S} -c ${S}/apache2/mod_security2.c ${S}/apache2/modsecurity.c ${S}/apache2/msc_logging.c ${S}/apache2/msc_multipart.c ${S}/apache2/msc_parsers.c ${S}/apache2/msc_pcre.c ${S}/apache2/msc_util.c ${S}/apache2/msc_xml ${S}/apache2/persist_dbm.c ${S}/apache2/re.c ${S}/apache2/re_actions.c ${S}/apache2/re_operators.c ${S}/apache2/re_tfns.c ${S}/apache2/re_variables.c ${S}/apache2/apache2_util.c ${S}/apache2/apache2_io.c ${S}/apache2/msc_reqbody.c ${S}/apache2/apache2_config.c"
APACHE2_MOD_FILE="apache2/.libs/${PN}2.so"
APACHE2_MOD_CONF="1.8.6/99_mod_security"
APACHE2_MOD_DEFINE="SECURITY"

DOCFILES="CHANGES httpd.conf.* INSTALL LICENSE README"
useq doc && DOCFILES="${DOCFILES} doc/modsecurity-apache-manual-1.9.pdf"

need_apache
