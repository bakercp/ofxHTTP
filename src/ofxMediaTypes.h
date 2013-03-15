/*==============================================================================
 
 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 =============================================================================*/

#pragma once

#include <map>

#include "Poco/RegularExpression.h"
#include "Poco/Net/MediaType.h"

using std::map;

using Poco::RegularExpression;
using Poco::Net::MediaType;

static string rawMimeTypes = "" \
"# http://svn.apache.org/viewvc/httpd/httpd/trunk/docs/conf/mime.types?view=markup\r" \
"#\r" \
"#\r" \
"# This file maps Internet media types to unique file extension(s).\r" \
"# Although created for httpd, this file is used by many software systems\r" \
"# and has been placed in the public domain for unlimited redisribution.\r" \
"#\r" \
"# The table below contains both registered and (common) unregistered types.\r" \
"# A type that has no unique extension can be ignored -- they are listed\r" \
"# here to guide configurations toward known types and to make it easier to\r" \
"# identify \"new\" types.  File extensions are also commonly used to indicate\r" \
"# content languages and encodings, so choose them carefully.\r" \
"#\r" \
"# Internet media types should be registered as described in RFC 4288.\r" \
"# The registry is at <http://www.iana.org/assignments/media-types/>.\r" \
"#\r" \
"# MIME type (lowercased)			Extensions\r" \
"# ============================================	==========\r" \
"# application/1d-interleaved-parityfec\r" \
"# application/3gpp-ims+xml\r" \
"# application/activemessage\r" \
"application/andrew-inset			ez\r" \
"# application/applefile\r" \
"application/applixware				aw\r" \
"application/atom+xml				atom\r" \
"application/atomcat+xml				atomcat\r" \
"# application/atomicmail\r" \
"application/atomsvc+xml				atomsvc\r" \
"# application/auth-policy+xml\r" \
"# application/batch-smtp\r" \
"# application/beep+xml\r" \
"# application/calendar+xml\r" \
"# application/cals-1840\r" \
"# application/ccmp+xml\r" \
"application/ccxml+xml				ccxml\r" \
"application/cdmi-capability			cdmia\r" \
"application/cdmi-container			cdmic\r" \
"application/cdmi-domain				cdmid\r" \
"application/cdmi-object				cdmio\r" \
"application/cdmi-queue				cdmiq\r" \
"# application/cea-2018+xml\r" \
"# application/cellml+xml\r" \
"# application/cfw\r" \
"# application/cnrp+xml\r" \
"# application/commonground\r" \
"# application/conference-info+xml\r" \
"# application/cpl+xml\r" \
"# application/csta+xml\r" \
"# application/cstadata+xml\r" \
"application/cu-seeme				cu\r" \
"# application/cybercash\r" \
"application/davmount+xml			davmount\r" \
"# application/dca-rft\r" \
"# application/dec-dx\r" \
"# application/dialog-info+xml\r" \
"# application/dicom\r" \
"# application/dns\r" \
"application/docbook+xml				dbk\r" \
"# application/dskpp+xml\r" \
"application/dssc+der				dssc\r" \
"application/dssc+xml				xdssc\r" \
"# application/dvcs\r" \
"application/ecmascript				ecma\r" \
"# application/edi-consent\r" \
"# application/edi-x12\r" \
"# application/edifact\r" \
"application/emma+xml				emma\r" \
"# application/epp+xml\r" \
"application/epub+zip				epub\r" \
"# application/eshop\r" \
"# application/example\r" \
"application/exi					exi\r" \
"# application/fastinfoset\r" \
"# application/fastsoap\r" \
"# application/fits\r" \
"application/font-tdpfr				pfr\r" \
"# application/framework-attributes+xml\r" \
"application/gml+xml				gml\r" \
"application/gpx+xml				gpx\r" \
"application/gxf					gxf\r" \
"# application/h224\r" \
"# application/held+xml\r" \
"# application/http\r" \
"application/hyperstudio				stk\r" \
"# application/ibe-key-request+xml\r" \
"# application/ibe-pkg-reply+xml\r" \
"# application/ibe-pp-data\r" \
"# application/iges\r" \
"# application/im-iscomposing+xml\r" \
"# application/index\r" \
"# application/index.cmd\r" \
"# application/index.obj\r" \
"# application/index.response\r" \
"# application/index.vnd\r" \
"application/inkml+xml				ink inkml\r" \
"# application/iotp\r" \
"application/ipfix				ipfix\r" \
"# application/ipp\r" \
"# application/isup\r" \
"application/java-archive			jar\r" \
"application/java-serialized-object		ser\r" \
"application/java-vm				class\r" \
"application/javascript				js\r" \
"application/json				json\r" \
"application/jsonml+json				jsonml\r" \
"# application/kpml-request+xml\r" \
"# application/kpml-response+xml\r" \
"application/lost+xml				lostxml\r" \
"application/mac-binhex40			hqx\r" \
"application/mac-compactpro			cpt\r" \
"# application/macwriteii\r" \
"application/mads+xml				mads\r" \
"application/marc				mrc\r" \
"application/marcxml+xml				mrcx\r" \
"application/mathematica				ma nb mb\r" \
"# application/mathml-content+xml\r" \
"# application/mathml-presentation+xml\r" \
"application/mathml+xml				mathml\r" \
"# application/mbms-associated-procedure-description+xml\r" \
"# application/mbms-deregister+xml\r" \
"# application/mbms-envelope+xml\r" \
"# application/mbms-msk+xml\r" \
"# application/mbms-msk-response+xml\r" \
"# application/mbms-protection-description+xml\r" \
"# application/mbms-reception-report+xml\r" \
"# application/mbms-register+xml\r" \
"# application/mbms-register-response+xml\r" \
"# application/mbms-user-service-description+xml\r" \
"application/mbox				mbox\r" \
"# application/media_control+xml\r" \
"application/mediaservercontrol+xml		mscml\r" \
"application/metalink+xml			metalink\r" \
"application/metalink4+xml			meta4\r" \
"application/mets+xml				mets\r" \
"# application/mikey\r" \
"application/mods+xml				mods\r" \
"# application/moss-keys\r" \
"# application/moss-signature\r" \
"# application/mosskey-data\r" \
"# application/mosskey-request\r" \
"application/mp21				m21 mp21\r" \
"application/mp4					mp4s\r" \
"# application/mpeg4-generic\r" \
"# application/mpeg4-iod\r" \
"# application/mpeg4-iod-xmt\r" \
"# application/msc-ivr+xml\r" \
"# application/msc-mixer+xml\r" \
"application/msword				doc dot\r" \
"application/mxf					mxf\r" \
"# application/nasdata\r" \
"# application/news-checkgroups\r" \
"# application/news-groupinfo\r" \
"# application/news-transmission\r" \
"# application/nss\r" \
"# application/ocsp-request\r" \
"# application/ocsp-response\r" \
"application/octet-stream	bin dms lrf mar so dist distz pkg bpk dump elc deploy\r" \
"application/oda					oda\r" \
"application/oebps-package+xml			opf\r" \
"application/ogg					ogx\r" \
"application/omdoc+xml				omdoc\r" \
"application/onenote				onetoc onetoc2 onetmp onepkg\r" \
"application/oxps				oxps\r" \
"# application/parityfec\r" \
"application/patch-ops-error+xml			xer\r" \
"application/pdf					pdf\r" \
"application/pgp-encrypted			pgp\r" \
"# application/pgp-keys\r" \
"application/pgp-signature			asc sig\r" \
"application/pics-rules				prf\r" \
"# application/pidf+xml\r" \
"# application/pidf-diff+xml\r" \
"application/pkcs10				p10\r" \
"application/pkcs7-mime				p7m p7c\r" \
"application/pkcs7-signature			p7s\r" \
"application/pkcs8				p8\r" \
"application/pkix-attr-cert			ac\r" \
"application/pkix-cert				cer\r" \
"application/pkix-crl				crl\r" \
"application/pkix-pkipath			pkipath\r" \
"application/pkixcmp				pki\r" \
"application/pls+xml				pls\r" \
"# application/poc-settings+xml\r" \
"application/postscript				ai eps ps\r" \
"# application/prs.alvestrand.titrax-sheet\r" \
"application/prs.cww				cww\r" \
"# application/prs.nprend\r" \
"# application/prs.plucker\r" \
"# application/prs.rdf-xml-crypt\r" \
"# application/prs.xsf+xml\r" \
"application/pskc+xml				pskcxml\r" \
"# application/qsig\r" \
"application/rdf+xml				rdf\r" \
"application/reginfo+xml				rif\r" \
"application/relax-ng-compact-syntax		rnc\r" \
"# application/remote-printing\r" \
"application/resource-lists+xml			rl\r" \
"application/resource-lists-diff+xml		rld\r" \
"# application/riscos\r" \
"# application/rlmi+xml\r" \
"application/rls-services+xml			rs\r" \
"application/rpki-ghostbusters			gbr\r" \
"application/rpki-manifest			mft\r" \
"application/rpki-roa				roa\r" \
"# application/rpki-updown\r" \
"application/rsd+xml				rsd\r" \
"application/rss+xml				rss\r" \
"application/rtf					rtf\r" \
"# application/rtx\r" \
"# application/samlassertion+xml\r" \
"# application/samlmetadata+xml\r" \
"application/sbml+xml				sbml\r" \
"application/scvp-cv-request			scq\r" \
"application/scvp-cv-response			scs\r" \
"application/scvp-vp-request			spq\r" \
"application/scvp-vp-response			spp\r" \
"application/sdp					sdp\r" \
"# application/set-payment\r" \
"application/set-payment-initiation		setpay\r" \
"# application/set-registration\r" \
"application/set-registration-initiation		setreg\r" \
"# application/sgml\r" \
"# application/sgml-open-catalog\r" \
"application/shf+xml				shf\r" \
"# application/sieve\r" \
"# application/simple-filter+xml\r" \
"# application/simple-message-summary\r" \
"# application/simplesymbolcontainer\r" \
"# application/slate\r" \
"# application/smil\r" \
"application/smil+xml				smi smil\r" \
"# application/soap+fastinfoset\r" \
"# application/soap+xml\r" \
"application/sparql-query			rq\r" \
"application/sparql-results+xml			srx\r" \
"# application/spirits-event+xml\r" \
"application/srgs				gram\r" \
"application/srgs+xml				grxml\r" \
"application/sru+xml				sru\r" \
"application/ssdl+xml				ssdl\r" \
"application/ssml+xml				ssml\r" \
"# application/tamp-apex-update\r" \
"# application/tamp-apex-update-confirm\r" \
"# application/tamp-community-update\r" \
"# application/tamp-community-update-confirm\r" \
"# application/tamp-error\r" \
"# application/tamp-sequence-adjust\r" \
"# application/tamp-sequence-adjust-confirm\r" \
"# application/tamp-status-query\r" \
"# application/tamp-status-response\r" \
"# application/tamp-update\r" \
"# application/tamp-update-confirm\r" \
"application/tei+xml				tei teicorpus\r" \
"application/thraud+xml				tfi\r" \
"# application/timestamp-query\r" \
"# application/timestamp-reply\r" \
"application/timestamped-data			tsd\r" \
"# application/tve-trigger\r" \
"# application/ulpfec\r" \
"# application/vcard+xml\r" \
"# application/vemmi\r" \
"# application/vividence.scriptfile\r" \
"# application/vnd.3gpp.bsf+xml\r" \
"application/vnd.3gpp.pic-bw-large		plb\r" \
"application/vnd.3gpp.pic-bw-small		psb\r" \
"application/vnd.3gpp.pic-bw-var			pvb\r" \
"# application/vnd.3gpp.sms\r" \
"# application/vnd.3gpp2.bcmcsinfo+xml\r" \
"# application/vnd.3gpp2.sms\r" \
"application/vnd.3gpp2.tcap			tcap\r" \
"application/vnd.3m.post-it-notes		pwn\r" \
"application/vnd.accpac.simply.aso		aso\r" \
"application/vnd.accpac.simply.imp		imp\r" \
"application/vnd.acucobol			acu\r" \
"application/vnd.acucorp				atc acutc\r" \
"application/vnd.adobe.air-application-installer-package+zip	air\r" \
"application/vnd.adobe.formscentral.fcdt		fcdt\r" \
"application/vnd.adobe.fxp			fxp fxpl\r" \
"# application/vnd.adobe.partial-upload\r" \
"application/vnd.adobe.xdp+xml			xdp\r" \
"application/vnd.adobe.xfdf			xfdf\r" \
"# application/vnd.aether.imp\r" \
"# application/vnd.ah-barcode\r" \
"application/vnd.ahead.space			ahead\r" \
"application/vnd.airzip.filesecure.azf		azf\r" \
"application/vnd.airzip.filesecure.azs		azs\r" \
"application/vnd.amazon.ebook			azw\r" \
"application/vnd.americandynamics.acc		acc\r" \
"application/vnd.amiga.ami			ami\r" \
"# application/vnd.amundsen.maze+xml\r" \
"application/vnd.android.package-archive		apk\r" \
"application/vnd.anser-web-certificate-issue-initiation	cii\r" \
"application/vnd.anser-web-funds-transfer-initiation	fti\r" \
"application/vnd.antix.game-component		atx\r" \
"application/vnd.apple.installer+xml		mpkg\r" \
"application/vnd.apple.mpegurl			m3u8\r" \
"# application/vnd.arastra.swi\r" \
"application/vnd.aristanetworks.swi		swi\r" \
"application/vnd.astraea-software.iota		iota\r" \
"application/vnd.audiograph			aep\r" \
"# application/vnd.autopackage\r" \
"# application/vnd.avistar+xml\r" \
"application/vnd.blueice.multipass		mpm\r" \
"# application/vnd.bluetooth.ep.oob\r" \
"application/vnd.bmi				bmi\r" \
"application/vnd.businessobjects			rep\r" \
"# application/vnd.cab-jscript\r" \
"# application/vnd.canon-cpdl\r" \
"# application/vnd.canon-lips\r" \
"# application/vnd.cendio.thinlinc.clientconf\r" \
"application/vnd.chemdraw+xml			cdxml\r" \
"application/vnd.chipnuts.karaoke-mmd		mmd\r" \
"application/vnd.cinderella			cdy\r" \
"# application/vnd.cirpack.isdn-ext\r" \
"application/vnd.claymore			cla\r" \
"application/vnd.cloanto.rp9			rp9\r" \
"application/vnd.clonk.c4group			c4g c4d c4f c4p c4u\r" \
"application/vnd.cluetrust.cartomobile-config		c11amc\r" \
"application/vnd.cluetrust.cartomobile-config-pkg	c11amz\r" \
"# application/vnd.collection+json\r" \
"# application/vnd.commerce-battelle\r" \
"application/vnd.commonspace			csp\r" \
"application/vnd.contact.cmsg			cdbcmsg\r" \
"application/vnd.cosmocaller			cmc\r" \
"application/vnd.crick.clicker			clkx\r" \
"application/vnd.crick.clicker.keyboard		clkk\r" \
"application/vnd.crick.clicker.palette		clkp\r" \
"application/vnd.crick.clicker.template		clkt\r" \
"application/vnd.crick.clicker.wordbank		clkw\r" \
"application/vnd.criticaltools.wbs+xml		wbs\r" \
"application/vnd.ctc-posml			pml\r" \
"# application/vnd.ctct.ws+xml\r" \
"# application/vnd.cups-pdf\r" \
"# application/vnd.cups-postscript\r" \
"application/vnd.cups-ppd			ppd\r" \
"# application/vnd.cups-raster\r" \
"# application/vnd.cups-raw\r" \
"# application/vnd.curl\r" \
"application/vnd.curl.car			car\r" \
"application/vnd.curl.pcurl			pcurl\r" \
"# application/vnd.cybank\r" \
"application/vnd.dart				dart\r" \
"application/vnd.data-vision.rdz			rdz\r" \
"application/vnd.dece.data			uvf uvvf uvd uvvd\r" \
"application/vnd.dece.ttml+xml			uvt uvvt\r" \
"application/vnd.dece.unspecified		uvx uvvx\r" \
"application/vnd.dece.zip			uvz uvvz\r" \
"application/vnd.denovo.fcselayout-link		fe_launch\r" \
"# application/vnd.dir-bi.plate-dl-nosuffix\r" \
"application/vnd.dna				dna\r" \
"application/vnd.dolby.mlp			mlp\r" \
"# application/vnd.dolby.mobile.1\r" \
"# application/vnd.dolby.mobile.2\r" \
"application/vnd.dpgraph				dpg\r" \
"application/vnd.dreamfactory			dfac\r" \
"application/vnd.ds-keypoint			kpxx\r" \
"application/vnd.dvb.ait				ait\r" \
"# application/vnd.dvb.dvbj\r" \
"# application/vnd.dvb.esgcontainer\r" \
"# application/vnd.dvb.ipdcdftnotifaccess\r" \
"# application/vnd.dvb.ipdcesgaccess\r" \
"# application/vnd.dvb.ipdcesgaccess2\r" \
"# application/vnd.dvb.ipdcesgpdd\r" \
"# application/vnd.dvb.ipdcroaming\r" \
"# application/vnd.dvb.iptv.alfec-base\r" \
"# application/vnd.dvb.iptv.alfec-enhancement\r" \
"# application/vnd.dvb.notif-aggregate-root+xml\r" \
"# application/vnd.dvb.notif-container+xml\r" \
"# application/vnd.dvb.notif-generic+xml\r" \
"# application/vnd.dvb.notif-ia-msglist+xml\r" \
"# application/vnd.dvb.notif-ia-registration-request+xml\r" \
"# application/vnd.dvb.notif-ia-registration-response+xml\r" \
"# application/vnd.dvb.notif-init+xml\r" \
"# application/vnd.dvb.pfr\r" \
"application/vnd.dvb.service			svc\r" \
"# application/vnd.dxr\r" \
"application/vnd.dynageo				geo\r" \
"# application/vnd.easykaraoke.cdgdownload\r" \
"# application/vnd.ecdis-update\r" \
"application/vnd.ecowin.chart			mag\r" \
"# application/vnd.ecowin.filerequest\r" \
"# application/vnd.ecowin.fileupdate\r" \
"# application/vnd.ecowin.series\r" \
"# application/vnd.ecowin.seriesrequest\r" \
"# application/vnd.ecowin.seriesupdate\r" \
"# application/vnd.emclient.accessrequest+xml\r" \
"application/vnd.enliven				nml\r" \
"# application/vnd.eprints.data+xml\r" \
"application/vnd.epson.esf			esf\r" \
"application/vnd.epson.msf			msf\r" \
"application/vnd.epson.quickanime		qam\r" \
"application/vnd.epson.salt			slt\r" \
"application/vnd.epson.ssf			ssf\r" \
"# application/vnd.ericsson.quickcall\r" \
"application/vnd.eszigno3+xml			es3 et3\r" \
"# application/vnd.etsi.aoc+xml\r" \
"# application/vnd.etsi.cug+xml\r" \
"# application/vnd.etsi.iptvcommand+xml\r" \
"# application/vnd.etsi.iptvdiscovery+xml\r" \
"# application/vnd.etsi.iptvprofile+xml\r" \
"# application/vnd.etsi.iptvsad-bc+xml\r" \
"# application/vnd.etsi.iptvsad-cod+xml\r" \
"# application/vnd.etsi.iptvsad-npvr+xml\r" \
"# application/vnd.etsi.iptvservice+xml\r" \
"# application/vnd.etsi.iptvsync+xml\r" \
"# application/vnd.etsi.iptvueprofile+xml\r" \
"# application/vnd.etsi.mcid+xml\r" \
"# application/vnd.etsi.overload-control-policy-dataset+xml\r" \
"# application/vnd.etsi.sci+xml\r" \
"# application/vnd.etsi.simservs+xml\r" \
"# application/vnd.etsi.tsl+xml\r" \
"# application/vnd.etsi.tsl.der\r" \
"# application/vnd.eudora.data\r" \
"application/vnd.ezpix-album			ez2\r" \
"application/vnd.ezpix-package			ez3\r" \
"# application/vnd.f-secure.mobile\r" \
"application/vnd.fdf				fdf\r" \
"application/vnd.fdsn.mseed			mseed\r" \
"application/vnd.fdsn.seed			seed dataless\r" \
"# application/vnd.ffsns\r" \
"# application/vnd.fints\r" \
"application/vnd.flographit			gph\r" \
"application/vnd.fluxtime.clip			ftc\r" \
"# application/vnd.font-fontforge-sfd\r" \
"application/vnd.framemaker			fm frame maker book\r" \
"application/vnd.frogans.fnc			fnc\r" \
"application/vnd.frogans.ltf			ltf\r" \
"application/vnd.fsc.weblaunch			fsc\r" \
"application/vnd.fujitsu.oasys			oas\r" \
"application/vnd.fujitsu.oasys2			oa2\r" \
"application/vnd.fujitsu.oasys3			oa3\r" \
"application/vnd.fujitsu.oasysgp			fg5\r" \
"application/vnd.fujitsu.oasysprs		bh2\r" \
"# application/vnd.fujixerox.art-ex\r" \
"# application/vnd.fujixerox.art4\r" \
"# application/vnd.fujixerox.hbpl\r" \
"application/vnd.fujixerox.ddd			ddd\r" \
"application/vnd.fujixerox.docuworks		xdw\r" \
"application/vnd.fujixerox.docuworks.binder	xbd\r" \
"# application/vnd.fut-misnet\r" \
"application/vnd.fuzzysheet			fzs\r" \
"application/vnd.genomatix.tuxedo		txd\r" \
"# application/vnd.geocube+xml\r" \
"application/vnd.geogebra.file			ggb\r" \
"application/vnd.geogebra.tool			ggt\r" \
"application/vnd.geometry-explorer		gex gre\r" \
"application/vnd.geonext				gxt\r" \
"application/vnd.geoplan				g2w\r" \
"application/vnd.geospace			g3w\r" \
"# application/vnd.globalplatform.card-content-mgt\r" \
"# application/vnd.globalplatform.card-content-mgt-response\r" \
"application/vnd.gmx				gmx\r" \
"application/vnd.google-earth.kml+xml		kml\r" \
"application/vnd.google-earth.kmz		kmz\r" \
"application/vnd.grafeq				gqf gqs\r" \
"# application/vnd.gridmp\r" \
"application/vnd.groove-account			gac\r" \
"application/vnd.groove-help			ghf\r" \
"application/vnd.groove-identity-message		gim\r" \
"application/vnd.groove-injector			grv\r" \
"application/vnd.groove-tool-message		gtm\r" \
"application/vnd.groove-tool-template		tpl\r" \
"application/vnd.groove-vcard			vcg\r" \
"# application/vnd.hal+json\r" \
"application/vnd.hal+xml				hal\r" \
"application/vnd.handheld-entertainment+xml	zmm\r" \
"application/vnd.hbci				hbci\r" \
"# application/vnd.hcl-bireports\r" \
"application/vnd.hhe.lesson-player		les\r" \
"application/vnd.hp-hpgl				hpgl\r" \
"application/vnd.hp-hpid				hpid\r" \
"application/vnd.hp-hps				hps\r" \
"application/vnd.hp-jlyt				jlt\r" \
"application/vnd.hp-pcl				pcl\r" \
"application/vnd.hp-pclxl			pclxl\r" \
"# application/vnd.httphone\r" \
"application/vnd.hydrostatix.sof-data		sfd-hdstx\r" \
"# application/vnd.hzn-3d-crossword\r" \
"# application/vnd.ibm.afplinedata\r" \
"# application/vnd.ibm.electronic-media\r" \
"application/vnd.ibm.minipay			mpy\r" \
"application/vnd.ibm.modcap			afp listafp list3820\r" \
"application/vnd.ibm.rights-management		irm\r" \
"application/vnd.ibm.secure-container		sc\r" \
"application/vnd.iccprofile			icc icm\r" \
"application/vnd.igloader			igl\r" \
"application/vnd.immervision-ivp			ivp\r" \
"application/vnd.immervision-ivu			ivu\r" \
"# application/vnd.informedcontrol.rms+xml\r" \
"# application/vnd.informix-visionary\r" \
"# application/vnd.infotech.project\r" \
"# application/vnd.infotech.project+xml\r" \
"# application/vnd.innopath.wamp.notification\r" \
"application/vnd.insors.igm			igm\r" \
"application/vnd.intercon.formnet		xpw xpx\r" \
"application/vnd.intergeo			i2g\r" \
"# application/vnd.intertrust.digibox\r" \
"# application/vnd.intertrust.nncp\r" \
"application/vnd.intu.qbo			qbo\r" \
"application/vnd.intu.qfx			qfx\r" \
"# application/vnd.iptc.g2.conceptitem+xml\r" \
"# application/vnd.iptc.g2.knowledgeitem+xml\r" \
"# application/vnd.iptc.g2.newsitem+xml\r" \
"# application/vnd.iptc.g2.newsmessage+xml\r" \
"# application/vnd.iptc.g2.packageitem+xml\r" \
"# application/vnd.iptc.g2.planningitem+xml\r" \
"application/vnd.ipunplugged.rcprofile		rcprofile\r" \
"application/vnd.irepository.package+xml		irp\r" \
"application/vnd.is-xpr				xpr\r" \
"application/vnd.isac.fcs			fcs\r" \
"application/vnd.jam				jam\r" \
"# application/vnd.japannet-directory-service\r" \
"# application/vnd.japannet-jpnstore-wakeup\r" \
"# application/vnd.japannet-payment-wakeup\r" \
"# application/vnd.japannet-registration\r" \
"# application/vnd.japannet-registration-wakeup\r" \
"# application/vnd.japannet-setstore-wakeup\r" \
"# application/vnd.japannet-verification\r" \
"# application/vnd.japannet-verification-wakeup\r" \
"application/vnd.jcp.javame.midlet-rms		rms\r" \
"application/vnd.jisp				jisp\r" \
"application/vnd.joost.joda-archive		joda\r" \
"application/vnd.kahootz				ktz ktr\r" \
"application/vnd.kde.karbon			karbon\r" \
"application/vnd.kde.kchart			chrt\r" \
"application/vnd.kde.kformula			kfo\r" \
"application/vnd.kde.kivio			flw\r" \
"application/vnd.kde.kontour			kon\r" \
"application/vnd.kde.kpresenter			kpr kpt\r" \
"application/vnd.kde.kspread			ksp\r" \
"application/vnd.kde.kword			kwd kwt\r" \
"application/vnd.kenameaapp			htke\r" \
"application/vnd.kidspiration			kia\r" \
"application/vnd.kinar				kne knp\r" \
"application/vnd.koan				skp skd skt skm\r" \
"application/vnd.kodak-descriptor		sse\r" \
"application/vnd.las.las+xml			lasxml\r" \
"# application/vnd.liberty-request+xml\r" \
"application/vnd.llamagraphics.life-balance.desktop	lbd\r" \
"application/vnd.llamagraphics.life-balance.exchange+xml	lbe\r" \
"application/vnd.lotus-1-2-3			123\r" \
"application/vnd.lotus-approach			apr\r" \
"application/vnd.lotus-freelance			pre\r" \
"application/vnd.lotus-notes			nsf\r" \
"application/vnd.lotus-organizer			org\r" \
"application/vnd.lotus-screencam			scm\r" \
"application/vnd.lotus-wordpro			lwp\r" \
"application/vnd.macports.portpkg		portpkg\r" \
"# application/vnd.marlin.drm.actiontoken+xml\r" \
"# application/vnd.marlin.drm.conftoken+xml\r" \
"# application/vnd.marlin.drm.license+xml\r" \
"# application/vnd.marlin.drm.mdcf\r" \
"application/vnd.mcd				mcd\r" \
"application/vnd.medcalcdata			mc1\r" \
"application/vnd.mediastation.cdkey		cdkey\r" \
"# application/vnd.meridian-slingshot\r" \
"application/vnd.mfer				mwf\r" \
"application/vnd.mfmp				mfm\r" \
"application/vnd.micrografx.flo			flo\r" \
"application/vnd.micrografx.igx			igx\r" \
"application/vnd.mif				mif\r" \
"# application/vnd.minisoft-hp3000-save\r" \
"# application/vnd.mitsubishi.misty-guard.trustweb\r" \
"application/vnd.mobius.daf			daf\r" \
"application/vnd.mobius.dis			dis\r" \
"application/vnd.mobius.mbk			mbk\r" \
"application/vnd.mobius.mqy			mqy\r" \
"application/vnd.mobius.msl			msl\r" \
"application/vnd.mobius.plc			plc\r" \
"application/vnd.mobius.txf			txf\r" \
"application/vnd.mophun.application		mpn\r" \
"application/vnd.mophun.certificate		mpc\r" \
"# application/vnd.motorola.flexsuite\r" \
"# application/vnd.motorola.flexsuite.adsi\r" \
"# application/vnd.motorola.flexsuite.fis\r" \
"# application/vnd.motorola.flexsuite.gotap\r" \
"# application/vnd.motorola.flexsuite.kmr\r" \
"# application/vnd.motorola.flexsuite.ttc\r" \
"# application/vnd.motorola.flexsuite.wem\r" \
"# application/vnd.motorola.iprm\r" \
"application/vnd.mozilla.xul+xml			xul\r" \
"application/vnd.ms-artgalry			cil\r" \
"# application/vnd.ms-asf\r" \
"application/vnd.ms-cab-compressed		cab\r" \
"# application/vnd.ms-color.iccprofile\r" \
"application/vnd.ms-excel			xls xlm xla xlc xlt xlw\r" \
"application/vnd.ms-excel.addin.macroenabled.12		xlam\r" \
"application/vnd.ms-excel.sheet.binary.macroenabled.12	xlsb\r" \
"application/vnd.ms-excel.sheet.macroenabled.12		xlsm\r" \
"application/vnd.ms-excel.template.macroenabled.12	xltm\r" \
"application/vnd.ms-fontobject			eot\r" \
"application/vnd.ms-htmlhelp			chm\r" \
"application/vnd.ms-ims				ims\r" \
"application/vnd.ms-lrm				lrm\r" \
"# application/vnd.ms-office.activex+xml\r" \
"application/vnd.ms-officetheme			thmx\r" \
"# application/vnd.ms-opentype\r" \
"# application/vnd.ms-package.obfuscated-opentype\r" \
"application/vnd.ms-pki.seccat			cat\r" \
"application/vnd.ms-pki.stl			stl\r" \
"# application/vnd.ms-playready.initiator+xml\r" \
"application/vnd.ms-powerpoint			ppt pps pot\r" \
"application/vnd.ms-powerpoint.addin.macroenabled.12		ppam\r" \
"application/vnd.ms-powerpoint.presentation.macroenabled.12	pptm\r" \
"application/vnd.ms-powerpoint.slide.macroenabled.12		sldm\r" \
"application/vnd.ms-powerpoint.slideshow.macroenabled.12		ppsm\r" \
"application/vnd.ms-powerpoint.template.macroenabled.12		potm\r" \
"# application/vnd.ms-printing.printticket+xml\r" \
"application/vnd.ms-project			mpp mpt\r" \
"# application/vnd.ms-tnef\r" \
"# application/vnd.ms-wmdrm.lic-chlg-req\r" \
"# application/vnd.ms-wmdrm.lic-resp\r" \
"# application/vnd.ms-wmdrm.meter-chlg-req\r" \
"# application/vnd.ms-wmdrm.meter-resp\r" \
"application/vnd.ms-word.document.macroenabled.12	docm\r" \
"application/vnd.ms-word.template.macroenabled.12	dotm\r" \
"application/vnd.ms-works			wps wks wcm wdb\r" \
"application/vnd.ms-wpl				wpl\r" \
"application/vnd.ms-xpsdocument			xps\r" \
"application/vnd.mseq				mseq\r" \
"# application/vnd.msign\r" \
"# application/vnd.multiad.creator\r" \
"# application/vnd.multiad.creator.cif\r" \
"# application/vnd.music-niff\r" \
"application/vnd.musician			mus\r" \
"application/vnd.muvee.style			msty\r" \
"application/vnd.mynfc				taglet\r" \
"# application/vnd.ncd.control\r" \
"# application/vnd.ncd.reference\r" \
"# application/vnd.nervana\r" \
"# application/vnd.netfpx\r" \
"application/vnd.neurolanguage.nlu		nlu\r" \
"application/vnd.nitf				ntf nitf\r" \
"application/vnd.noblenet-directory		nnd\r" \
"application/vnd.noblenet-sealer			nns\r" \
"application/vnd.noblenet-web			nnw\r" \
"# application/vnd.nokia.catalogs\r" \
"# application/vnd.nokia.conml+wbxml\r" \
"# application/vnd.nokia.conml+xml\r" \
"# application/vnd.nokia.isds-radio-presets\r" \
"# application/vnd.nokia.iptv.config+xml\r" \
"# application/vnd.nokia.landmark+wbxml\r" \
"# application/vnd.nokia.landmark+xml\r" \
"# application/vnd.nokia.landmarkcollection+xml\r" \
"# application/vnd.nokia.n-gage.ac+xml\r" \
"application/vnd.nokia.n-gage.data		ngdat\r" \
"application/vnd.nokia.n-gage.symbian.install	n-gage\r" \
"# application/vnd.nokia.ncd\r" \
"# application/vnd.nokia.pcd+wbxml\r" \
"# application/vnd.nokia.pcd+xml\r" \
"application/vnd.nokia.radio-preset		rpst\r" \
"application/vnd.nokia.radio-presets		rpss\r" \
"application/vnd.novadigm.edm			edm\r" \
"application/vnd.novadigm.edx			edx\r" \
"application/vnd.novadigm.ext			ext\r" \
"# application/vnd.ntt-local.file-transfer\r" \
"# application/vnd.ntt-local.sip-ta_remote\r" \
"# application/vnd.ntt-local.sip-ta_tcp_stream\r" \
"application/vnd.oasis.opendocument.chart		odc\r" \
"application/vnd.oasis.opendocument.chart-template	otc\r" \
"application/vnd.oasis.opendocument.database		odb\r" \
"application/vnd.oasis.opendocument.formula		odf\r" \
"application/vnd.oasis.opendocument.formula-template	odft\r" \
"application/vnd.oasis.opendocument.graphics		odg\r" \
"application/vnd.oasis.opendocument.graphics-template	otg\r" \
"application/vnd.oasis.opendocument.image		odi\r" \
"application/vnd.oasis.opendocument.image-template	oti\r" \
"application/vnd.oasis.opendocument.presentation		odp\r" \
"application/vnd.oasis.opendocument.presentation-template	otp\r" \
"application/vnd.oasis.opendocument.spreadsheet		ods\r" \
"application/vnd.oasis.opendocument.spreadsheet-template	ots\r" \
"application/vnd.oasis.opendocument.text			odt\r" \
"application/vnd.oasis.opendocument.text-master		odm\r" \
"application/vnd.oasis.opendocument.text-template	ott\r" \
"application/vnd.oasis.opendocument.text-web		oth\r" \
"# application/vnd.obn\r" \
"# application/vnd.oftn.l10n+json\r" \
"# application/vnd.oipf.contentaccessdownload+xml\r" \
"# application/vnd.oipf.contentaccessstreaming+xml\r" \
"# application/vnd.oipf.cspg-hexbinary\r" \
"# application/vnd.oipf.dae.svg+xml\r" \
"# application/vnd.oipf.dae.xhtml+xml\r" \
"# application/vnd.oipf.mippvcontrolmessage+xml\r" \
"# application/vnd.oipf.pae.gem\r" \
"# application/vnd.oipf.spdiscovery+xml\r" \
"# application/vnd.oipf.spdlist+xml\r" \
"# application/vnd.oipf.ueprofile+xml\r" \
"# application/vnd.oipf.userprofile+xml\r" \
"application/vnd.olpc-sugar			xo\r" \
"# application/vnd.oma-scws-config\r" \
"# application/vnd.oma-scws-http-request\r" \
"# application/vnd.oma-scws-http-response\r" \
"# application/vnd.oma.bcast.associated-procedure-parameter+xml\r" \
"# application/vnd.oma.bcast.drm-trigger+xml\r" \
"# application/vnd.oma.bcast.imd+xml\r" \
"# application/vnd.oma.bcast.ltkm\r" \
"# application/vnd.oma.bcast.notification+xml\r" \
"# application/vnd.oma.bcast.provisioningtrigger\r" \
"# application/vnd.oma.bcast.sgboot\r" \
"# application/vnd.oma.bcast.sgdd+xml\r" \
"# application/vnd.oma.bcast.sgdu\r" \
"# application/vnd.oma.bcast.simple-symbol-container\r" \
"# application/vnd.oma.bcast.smartcard-trigger+xml\r" \
"# application/vnd.oma.bcast.sprov+xml\r" \
"# application/vnd.oma.bcast.stkm\r" \
"# application/vnd.oma.cab-address-book+xml\r" \
"# application/vnd.oma.cab-feature-handler+xml\r" \
"# application/vnd.oma.cab-pcc+xml\r" \
"# application/vnd.oma.cab-user-prefs+xml\r" \
"# application/vnd.oma.dcd\r" \
"# application/vnd.oma.dcdc\r" \
"application/vnd.oma.dd2+xml			dd2\r" \
"# application/vnd.oma.drm.risd+xml\r" \
"# application/vnd.oma.group-usage-list+xml\r" \
"# application/vnd.oma.pal+xml\r" \
"# application/vnd.oma.poc.detailed-progress-report+xml\r" \
"# application/vnd.oma.poc.final-report+xml\r" \
"# application/vnd.oma.poc.groups+xml\r" \
"# application/vnd.oma.poc.invocation-descriptor+xml\r" \
"# application/vnd.oma.poc.optimized-progress-report+xml\r" \
"# application/vnd.oma.push\r" \
"# application/vnd.oma.scidm.messages+xml\r" \
"# application/vnd.oma.xcap-directory+xml\r" \
"# application/vnd.omads-email+xml\r" \
"# application/vnd.omads-file+xml\r" \
"# application/vnd.omads-folder+xml\r" \
"# application/vnd.omaloc-supl-init\r" \
"application/vnd.openofficeorg.extension		oxt\r" \
"# application/vnd.openxmlformats-officedocument.custom-properties+xml\r" \
"# application/vnd.openxmlformats-officedocument.customxmlproperties+xml\r" \
"# application/vnd.openxmlformats-officedocument.drawing+xml\r" \
"# application/vnd.openxmlformats-officedocument.drawingml.chart+xml\r" \
"# application/vnd.openxmlformats-officedocument.drawingml.chartshapes+xml\r" \
"# application/vnd.openxmlformats-officedocument.drawingml.diagramcolors+xml\r" \
"# application/vnd.openxmlformats-officedocument.drawingml.diagramdata+xml\r" \
"# application/vnd.openxmlformats-officedocument.drawingml.diagramlayout+xml\r" \
"# application/vnd.openxmlformats-officedocument.drawingml.diagramstyle+xml\r" \
"# application/vnd.openxmlformats-officedocument.extended-properties+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.commentauthors+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.comments+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.handoutmaster+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.notesmaster+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.notesslide+xml\r" \
"application/vnd.openxmlformats-officedocument.presentationml.presentation	pptx\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.presprops+xml\r" \
"application/vnd.openxmlformats-officedocument.presentationml.slide	sldx\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.slide+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.slidelayout+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.slidemaster+xml\r" \
"application/vnd.openxmlformats-officedocument.presentationml.slideshow	ppsx\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.slideupdateinfo+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.tablestyles+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.tags+xml\r" \
"application/vnd.openxmlformats-officedocument.presentationml.template	potx\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.template.main+xml\r" \
"# application/vnd.openxmlformats-officedocument.presentationml.viewprops+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.calcchain+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.chartsheet+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.comments+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.connections+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.dialogsheet+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.externallink+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.pivotcachedefinition+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.pivotcacherecords+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.pivottable+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.querytable+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.revisionheaders+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.revisionlog+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.sharedstrings+xml\r" \
"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet	xlsx\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.sheetmetadata+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.table+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.tablesinglecells+xml\r" \
"application/vnd.openxmlformats-officedocument.spreadsheetml.template	xltx\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.usernames+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.volatiledependencies+xml\r" \
"# application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml\r" \
"# application/vnd.openxmlformats-officedocument.theme+xml\r" \
"# application/vnd.openxmlformats-officedocument.themeoverride+xml\r" \
"# application/vnd.openxmlformats-officedocument.vmldrawing\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.comments+xml\r" \
"application/vnd.openxmlformats-officedocument.wordprocessingml.document	docx\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.document.glossary+xml\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.fonttable+xml\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\r" \
"application/vnd.openxmlformats-officedocument.wordprocessingml.template	dotx\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml\r" \
"# application/vnd.openxmlformats-officedocument.wordprocessingml.websettings+xml\r" \
"# application/vnd.openxmlformats-package.core-properties+xml\r" \
"# application/vnd.openxmlformats-package.digital-signature-xmlsignature+xml\r" \
"# application/vnd.openxmlformats-package.relationships+xml\r" \
"# application/vnd.quobject-quoxdocument\r" \
"# application/vnd.osa.netdeploy\r" \
"application/vnd.osgeo.mapguide.package		mgp\r" \
"# application/vnd.osgi.bundle\r" \
"application/vnd.osgi.dp				dp\r" \
"application/vnd.osgi.subsystem			esa\r" \
"# application/vnd.otps.ct-kip+xml\r" \
"application/vnd.palm				pdb pqa oprc\r" \
"# application/vnd.paos.xml\r" \
"application/vnd.pawaafile			paw\r" \
"application/vnd.pg.format			str\r" \
"application/vnd.pg.osasli			ei6\r" \
"# application/vnd.piaccess.application-licence\r" \
"application/vnd.picsel				efif\r" \
"application/vnd.pmi.widget			wg\r" \
"# application/vnd.poc.group-advertisement+xml\r" \
"application/vnd.pocketlearn			plf\r" \
"application/vnd.powerbuilder6			pbd\r" \
"# application/vnd.powerbuilder6-s\r" \
"# application/vnd.powerbuilder7\r" \
"# application/vnd.powerbuilder7-s\r" \
"# application/vnd.powerbuilder75\r" \
"# application/vnd.powerbuilder75-s\r" \
"# application/vnd.preminet\r" \
"application/vnd.previewsystems.box		box\r" \
"application/vnd.proteus.magazine		mgz\r" \
"application/vnd.publishare-delta-tree		qps\r" \
"application/vnd.pvi.ptid1			ptid\r" \
"# application/vnd.pwg-multiplexed\r" \
"# application/vnd.pwg-xhtml-print+xml\r" \
"# application/vnd.qualcomm.brew-app-res\r" \
"application/vnd.quark.quarkxpress		qxd qxt qwd qwt qxl qxb\r" \
"# application/vnd.radisys.moml+xml\r" \
"# application/vnd.radisys.msml+xml\r" \
"# application/vnd.radisys.msml-audit+xml\r" \
"# application/vnd.radisys.msml-audit-conf+xml\r" \
"# application/vnd.radisys.msml-audit-conn+xml\r" \
"# application/vnd.radisys.msml-audit-dialog+xml\r" \
"# application/vnd.radisys.msml-audit-stream+xml\r" \
"# application/vnd.radisys.msml-conf+xml\r" \
"# application/vnd.radisys.msml-dialog+xml\r" \
"# application/vnd.radisys.msml-dialog-base+xml\r" \
"# application/vnd.radisys.msml-dialog-fax-detect+xml\r" \
"# application/vnd.radisys.msml-dialog-fax-sendrecv+xml\r" \
"# application/vnd.radisys.msml-dialog-group+xml\r" \
"# application/vnd.radisys.msml-dialog-speech+xml\r" \
"# application/vnd.radisys.msml-dialog-transform+xml\r" \
"# application/vnd.rainstor.data\r" \
"# application/vnd.rapid\r" \
"application/vnd.realvnc.bed			bed\r" \
"application/vnd.recordare.musicxml		mxl\r" \
"application/vnd.recordare.musicxml+xml		musicxml\r" \
"# application/vnd.renlearn.rlprint\r" \
"application/vnd.rig.cryptonote			cryptonote\r" \
"application/vnd.rim.cod				cod\r" \
"application/vnd.rn-realmedia			rm\r" \
"application/vnd.rn-realmedia-vbr		rmvb\r" \
"application/vnd.route66.link66+xml		link66\r" \
"# application/vnd.rs-274x\r" \
"# application/vnd.ruckus.download\r" \
"# application/vnd.s3sms\r" \
"application/vnd.sailingtracker.track		st\r" \
"# application/vnd.sbm.cid\r" \
"# application/vnd.sbm.mid2\r" \
"# application/vnd.scribus\r" \
"# application/vnd.sealed.3df\r" \
"# application/vnd.sealed.csf\r" \
"# application/vnd.sealed.doc\r" \
"# application/vnd.sealed.eml\r" \
"# application/vnd.sealed.mht\r" \
"# application/vnd.sealed.net\r" \
"# application/vnd.sealed.ppt\r" \
"# application/vnd.sealed.tiff\r" \
"# application/vnd.sealed.xls\r" \
"# application/vnd.sealedmedia.softseal.html\r" \
"# application/vnd.sealedmedia.softseal.pdf\r" \
"application/vnd.seemail				see\r" \
"application/vnd.sema				sema\r" \
"application/vnd.semd				semd\r" \
"application/vnd.semf				semf\r" \
"application/vnd.shana.informed.formdata		ifm\r" \
"application/vnd.shana.informed.formtemplate	itp\r" \
"application/vnd.shana.informed.interchange	iif\r" \
"application/vnd.shana.informed.package		ipk\r" \
"application/vnd.simtech-mindmapper		twd twds\r" \
"application/vnd.smaf				mmf\r" \
"# application/vnd.smart.notebook\r" \
"application/vnd.smart.teacher			teacher\r" \
"# application/vnd.software602.filler.form+xml\r" \
"# application/vnd.software602.filler.form-xml-zip\r" \
"application/vnd.solent.sdkm+xml			sdkm sdkd\r" \
"application/vnd.spotfire.dxp			dxp\r" \
"application/vnd.spotfire.sfs			sfs\r" \
"# application/vnd.sss-cod\r" \
"# application/vnd.sss-dtf\r" \
"# application/vnd.sss-ntf\r" \
"application/vnd.stardivision.calc		sdc\r" \
"application/vnd.stardivision.draw		sda\r" \
"application/vnd.stardivision.impress		sdd\r" \
"application/vnd.stardivision.math		smf\r" \
"application/vnd.stardivision.writer		sdw vor\r" \
"application/vnd.stardivision.writer-global	sgl\r" \
"application/vnd.stepmania.package		smzip\r" \
"application/vnd.stepmania.stepchart		sm\r" \
"# application/vnd.street-stream\r" \
"application/vnd.sun.xml.calc			sxc\r" \
"application/vnd.sun.xml.calc.template		stc\r" \
"application/vnd.sun.xml.draw			sxd\r" \
"application/vnd.sun.xml.draw.template		std\r" \
"application/vnd.sun.xml.impress			sxi\r" \
"application/vnd.sun.xml.impress.template	sti\r" \
"application/vnd.sun.xml.math			sxm\r" \
"application/vnd.sun.xml.writer			sxw\r" \
"application/vnd.sun.xml.writer.global		sxg\r" \
"application/vnd.sun.xml.writer.template		stw\r" \
"# application/vnd.sun.wadl+xml\r" \
"application/vnd.sus-calendar			sus susp\r" \
"application/vnd.svd				svd\r" \
"# application/vnd.swiftview-ics\r" \
"application/vnd.symbian.install			sis sisx\r" \
"application/vnd.syncml+xml			xsm\r" \
"application/vnd.syncml.dm+wbxml			bdm\r" \
"application/vnd.syncml.dm+xml			xdm\r" \
"# application/vnd.syncml.dm.notification\r" \
"# application/vnd.syncml.ds.notification\r" \
"application/vnd.tao.intent-module-archive	tao\r" \
"application/vnd.tcpdump.pcap			pcap cap dmp\r" \
"application/vnd.tmobile-livetv			tmo\r" \
"application/vnd.trid.tpt			tpt\r" \
"application/vnd.triscape.mxs			mxs\r" \
"application/vnd.trueapp				tra\r" \
"# application/vnd.truedoc\r" \
"# application/vnd.ubisoft.webplayer\r" \
"application/vnd.ufdl				ufd ufdl\r" \
"application/vnd.uiq.theme			utz\r" \
"application/vnd.umajin				umj\r" \
"application/vnd.unity				unityweb\r" \
"application/vnd.uoml+xml			uoml\r" \
"# application/vnd.uplanet.alert\r" \
"# application/vnd.uplanet.alert-wbxml\r" \
"# application/vnd.uplanet.bearer-choice\r" \
"# application/vnd.uplanet.bearer-choice-wbxml\r" \
"# application/vnd.uplanet.cacheop\r" \
"# application/vnd.uplanet.cacheop-wbxml\r" \
"# application/vnd.uplanet.channel\r" \
"# application/vnd.uplanet.channel-wbxml\r" \
"# application/vnd.uplanet.list\r" \
"# application/vnd.uplanet.list-wbxml\r" \
"# application/vnd.uplanet.listcmd\r" \
"# application/vnd.uplanet.listcmd-wbxml\r" \
"# application/vnd.uplanet.signal\r" \
"application/vnd.vcx				vcx\r" \
"# application/vnd.vd-study\r" \
"# application/vnd.vectorworks\r" \
"# application/vnd.verimatrix.vcas\r" \
"# application/vnd.vidsoft.vidconference\r" \
"application/vnd.visio				vsd vst vss vsw\r" \
"application/vnd.visionary			vis\r" \
"# application/vnd.vividence.scriptfile\r" \
"application/vnd.vsf				vsf\r" \
"# application/vnd.wap.sic\r" \
"# application/vnd.wap.slc\r" \
"application/vnd.wap.wbxml			wbxml\r" \
"application/vnd.wap.wmlc			wmlc\r" \
"application/vnd.wap.wmlscriptc			wmlsc\r" \
"application/vnd.webturbo			wtb\r" \
"# application/vnd.wfa.wsc\r" \
"# application/vnd.wmc\r" \
"# application/vnd.wmf.bootstrap\r" \
"# application/vnd.wolfram.mathematica\r" \
"# application/vnd.wolfram.mathematica.package\r" \
"application/vnd.wolfram.player			nbp\r" \
"application/vnd.wordperfect			wpd\r" \
"application/vnd.wqd				wqd\r" \
"# application/vnd.wrq-hp3000-labelled\r" \
"application/vnd.wt.stf				stf\r" \
"# application/vnd.wv.csp+wbxml\r" \
"# application/vnd.wv.csp+xml\r" \
"# application/vnd.wv.ssp+xml\r" \
"application/vnd.xara				xar\r" \
"application/vnd.xfdl				xfdl\r" \
"# application/vnd.xfdl.webform\r" \
"# application/vnd.xmi+xml\r" \
"# application/vnd.xmpie.cpkg\r" \
"# application/vnd.xmpie.dpkg\r" \
"# application/vnd.xmpie.plan\r" \
"# application/vnd.xmpie.ppkg\r" \
"# application/vnd.xmpie.xlim\r" \
"application/vnd.yamaha.hv-dic			hvd\r" \
"application/vnd.yamaha.hv-script		hvs\r" \
"application/vnd.yamaha.hv-voice			hvp\r" \
"application/vnd.yamaha.openscoreformat			osf\r" \
"application/vnd.yamaha.openscoreformat.osfpvg+xml	osfpvg\r" \
"# application/vnd.yamaha.remote-setup\r" \
"application/vnd.yamaha.smaf-audio		saf\r" \
"application/vnd.yamaha.smaf-phrase		spf\r" \
"# application/vnd.yamaha.through-ngn\r" \
"# application/vnd.yamaha.tunnel-udpencap\r" \
"application/vnd.yellowriver-custom-menu		cmp\r" \
"application/vnd.zul				zir zirz\r" \
"application/vnd.zzazz.deck+xml			zaz\r" \
"application/voicexml+xml			vxml\r" \
"# application/vq-rtcpxr\r" \
"# application/watcherinfo+xml\r" \
"# application/whoispp-query\r" \
"# application/whoispp-response\r" \
"application/widget				wgt\r" \
"application/winhlp				hlp\r" \
"# application/wita\r" \
"# application/wordperfect5.1\r" \
"application/wsdl+xml				wsdl\r" \
"application/wspolicy+xml			wspolicy\r" \
"application/x-7z-compressed			7z\r" \
"application/x-abiword				abw\r" \
"application/x-ace-compressed			ace\r" \
"# application/x-amf\r" \
"application/x-apple-diskimage			dmg\r" \
"application/x-authorware-bin			aab x32 u32 vox\r" \
"application/x-authorware-map			aam\r" \
"application/x-authorware-seg			aas\r" \
"application/x-bcpio				bcpio\r" \
"application/x-bittorrent			torrent\r" \
"application/x-blorb				blb blorb\r" \
"application/x-bzip				bz\r" \
"application/x-bzip2				bz2 boz\r" \
"application/x-cbr				cbr cba cbt cbz cb7\r" \
"application/x-cdlink				vcd\r" \
"application/x-cfs-compressed			cfs\r" \
"application/x-chat				chat\r" \
"application/x-chess-pgn				pgn\r" \
"application/x-conference			nsc\r" \
"# application/x-compress\r" \
"application/x-cpio				cpio\r" \
"application/x-csh				csh\r" \
"application/x-debian-package			deb udeb\r" \
"application/x-dgc-compressed			dgc\r" \
"application/x-director			dir dcr dxr cst cct cxt w3d fgd swa\r" \
"application/x-doom				wad\r" \
"application/x-dtbncx+xml			ncx\r" \
"application/x-dtbook+xml			dtb\r" \
"application/x-dtbresource+xml			res\r" \
"application/x-dvi				dvi\r" \
"application/x-envoy				evy\r" \
"application/x-eva				eva\r" \
"application/x-font-bdf				bdf\r" \
"# application/x-font-dos\r" \
"# application/x-font-framemaker\r" \
"application/x-font-ghostscript			gsf\r" \
"# application/x-font-libgrx\r" \
"application/x-font-linux-psf			psf\r" \
"application/x-font-otf				otf\r" \
"application/x-font-pcf				pcf\r" \
"application/x-font-snf				snf\r" \
"# application/x-font-speedo\r" \
"# application/x-font-sunos-news\r" \
"application/x-font-ttf				ttf ttc\r" \
"application/x-font-type1			pfa pfb pfm afm\r" \
"application/x-font-woff				woff\r" \
"# application/x-font-vfont\r" \
"application/x-freearc				arc\r" \
"application/x-futuresplash			spl\r" \
"application/x-gca-compressed			gca\r" \
"application/x-glulx				ulx\r" \
"application/x-gnumeric				gnumeric\r" \
"application/x-gramps-xml			gramps\r" \
"application/x-gtar				gtar\r" \
"# application/x-gzip\r" \
"application/x-hdf				hdf\r" \
"application/x-install-instructions		install\r" \
"application/x-iso9660-image			iso\r" \
"application/x-java-jnlp-file			jnlp\r" \
"application/x-latex				latex\r" \
"application/x-lzh-compressed			lzh lha\r" \
"application/x-mie				mie\r" \
"application/x-mobipocket-ebook			prc mobi\r" \
"application/x-ms-application			application\r" \
"application/x-ms-shortcut			lnk\r" \
"application/x-ms-wmd				wmd\r" \
"application/x-ms-wmz				wmz\r" \
"application/x-ms-xbap				xbap\r" \
"application/x-msaccess				mdb\r" \
"application/x-msbinder				obd\r" \
"application/x-mscardfile			crd\r" \
"application/x-msclip				clp\r" \
"application/x-msdownload			exe dll com bat msi\r" \
"application/x-msmediaview			mvb m13 m14\r" \
"application/x-msmetafile			wmf wmz emf emz\r" \
"application/x-msmoney				mny\r" \
"application/x-mspublisher			pub\r" \
"application/x-msschedule			scd\r" \
"application/x-msterminal			trm\r" \
"application/x-mswrite				wri\r" \
"application/x-netcdf				nc cdf\r" \
"application/x-nzb				nzb\r" \
"application/x-pkcs12				p12 pfx\r" \
"application/x-pkcs7-certificates		p7b spc\r" \
"application/x-pkcs7-certreqresp			p7r\r" \
"application/x-rar-compressed			rar\r" \
"application/x-research-info-systems		ris\r" \
"application/x-sh				sh\r" \
"application/x-shar				shar\r" \
"application/x-shockwave-flash			swf\r" \
"application/x-silverlight-app			xap\r" \
"application/x-sql				sql\r" \
"application/x-stuffit				sit\r" \
"application/x-stuffitx				sitx\r" \
"application/x-subrip				srt\r" \
"application/x-sv4cpio				sv4cpio\r" \
"application/x-sv4crc				sv4crc\r" \
"application/x-t3vm-image			t3\r" \
"application/x-tads				gam\r" \
"application/x-tar				tar\r" \
"application/x-tcl				tcl\r" \
"application/x-tex				tex\r" \
"application/x-tex-tfm				tfm\r" \
"application/x-texinfo				texinfo texi\r" \
"application/x-tgif				obj\r" \
"application/x-ustar				ustar\r" \
"application/x-wais-source			src\r" \
"application/x-x509-ca-cert			der crt\r" \
"application/x-xfig				fig\r" \
"application/x-xliff+xml				xlf\r" \
"application/x-xpinstall				xpi\r" \
"application/x-xz				xz\r" \
"application/x-zmachine				z1 z2 z3 z4 z5 z6 z7 z8\r" \
"# application/x400-bp\r" \
"application/xaml+xml				xaml\r" \
"# application/xcap-att+xml\r" \
"# application/xcap-caps+xml\r" \
"application/xcap-diff+xml			xdf\r" \
"# application/xcap-el+xml\r" \
"# application/xcap-error+xml\r" \
"# application/xcap-ns+xml\r" \
"# application/xcon-conference-info-diff+xml\r" \
"# application/xcon-conference-info+xml\r" \
"application/xenc+xml				xenc\r" \
"application/xhtml+xml				xhtml xht\r" \
"# application/xhtml-voice+xml\r" \
"application/xml					xml xsl\r" \
"application/xml-dtd				dtd\r" \
"# application/xml-external-parsed-entity\r" \
"# application/xmpp+xml\r" \
"application/xop+xml				xop\r" \
"application/xproc+xml				xpl\r" \
"application/xslt+xml				xslt\r" \
"application/xspf+xml				xspf\r" \
"application/xv+xml				mxml xhvml xvml xvm\r" \
"application/yang				yang\r" \
"application/yin+xml				yin\r" \
"application/zip					zip\r" \
"# audio/1d-interleaved-parityfec\r" \
"# audio/32kadpcm\r" \
"# audio/3gpp\r" \
"# audio/3gpp2\r" \
"# audio/ac3\r" \
"audio/adpcm					adp\r" \
"# audio/amr\r" \
"# audio/amr-wb\r" \
"# audio/amr-wb+\r" \
"# audio/asc\r" \
"# audio/atrac-advanced-lossless\r" \
"# audio/atrac-x\r" \
"# audio/atrac3\r" \
"audio/basic					au snd\r" \
"# audio/bv16\r" \
"# audio/bv32\r" \
"# audio/clearmode\r" \
"# audio/cn\r" \
"# audio/dat12\r" \
"# audio/dls\r" \
"# audio/dsr-es201108\r" \
"# audio/dsr-es202050\r" \
"# audio/dsr-es202211\r" \
"# audio/dsr-es202212\r" \
"# audio/dv\r" \
"# audio/dvi4\r" \
"# audio/eac3\r" \
"# audio/evrc\r" \
"# audio/evrc-qcp\r" \
"# audio/evrc0\r" \
"# audio/evrc1\r" \
"# audio/evrcb\r" \
"# audio/evrcb0\r" \
"# audio/evrcb1\r" \
"# audio/evrcwb\r" \
"# audio/evrcwb0\r" \
"# audio/evrcwb1\r" \
"# audio/example\r" \
"# audio/fwdred\r" \
"# audio/g719\r" \
"# audio/g722\r" \
"# audio/g7221\r" \
"# audio/g723\r" \
"# audio/g726-16\r" \
"# audio/g726-24\r" \
"# audio/g726-32\r" \
"# audio/g726-40\r" \
"# audio/g728\r" \
"# audio/g729\r" \
"# audio/g7291\r" \
"# audio/g729d\r" \
"# audio/g729e\r" \
"# audio/gsm\r" \
"# audio/gsm-efr\r" \
"# audio/gsm-hr-08\r" \
"# audio/ilbc\r" \
"# audio/ip-mr_v2.5\r" \
"# audio/isac\r" \
"# audio/l16\r" \
"# audio/l20\r" \
"# audio/l24\r" \
"# audio/l8\r" \
"# audio/lpc\r" \
"audio/midi					mid midi kar rmi\r" \
"# audio/mobile-xmf\r" \
"audio/mp4					mp4a\r" \
"# audio/mp4a-latm\r" \
"# audio/mpa\r" \
"# audio/mpa-robust\r" \
"audio/mpeg					mpga mp2 mp2a mp3 m2a m3a\r" \
"# audio/mpeg4-generic\r" \
"# audio/musepack\r" \
"audio/ogg					oga ogg spx\r" \
"# audio/opus\r" \
"# audio/parityfec\r" \
"# audio/pcma\r" \
"# audio/pcma-wb\r" \
"# audio/pcmu-wb\r" \
"# audio/pcmu\r" \
"# audio/prs.sid\r" \
"# audio/qcelp\r" \
"# audio/red\r" \
"# audio/rtp-enc-aescm128\r" \
"# audio/rtp-midi\r" \
"# audio/rtx\r" \
"audio/s3m					s3m\r" \
"audio/silk					sil\r" \
"# audio/smv\r" \
"# audio/smv0\r" \
"# audio/smv-qcp\r" \
"# audio/sp-midi\r" \
"# audio/speex\r" \
"# audio/t140c\r" \
"# audio/t38\r" \
"# audio/telephone-event\r" \
"# audio/tone\r" \
"# audio/uemclip\r" \
"# audio/ulpfec\r" \
"# audio/vdvi\r" \
"# audio/vmr-wb\r" \
"# audio/vnd.3gpp.iufp\r" \
"# audio/vnd.4sb\r" \
"# audio/vnd.audiokoz\r" \
"# audio/vnd.celp\r" \
"# audio/vnd.cisco.nse\r" \
"# audio/vnd.cmles.radio-events\r" \
"# audio/vnd.cns.anp1\r" \
"# audio/vnd.cns.inf1\r" \
"audio/vnd.dece.audio				uva uvva\r" \
"audio/vnd.digital-winds				eol\r" \
"# audio/vnd.dlna.adts\r" \
"# audio/vnd.dolby.heaac.1\r" \
"# audio/vnd.dolby.heaac.2\r" \
"# audio/vnd.dolby.mlp\r" \
"# audio/vnd.dolby.mps\r" \
"# audio/vnd.dolby.pl2\r" \
"# audio/vnd.dolby.pl2x\r" \
"# audio/vnd.dolby.pl2z\r" \
"# audio/vnd.dolby.pulse.1\r" \
"audio/vnd.dra					dra\r" \
"audio/vnd.dts					dts\r" \
"audio/vnd.dts.hd				dtshd\r" \
"# audio/vnd.dvb.file\r" \
"# audio/vnd.everad.plj\r" \
"# audio/vnd.hns.audio\r" \
"audio/vnd.lucent.voice				lvp\r" \
"audio/vnd.ms-playready.media.pya		pya\r" \
"# audio/vnd.nokia.mobile-xmf\r" \
"# audio/vnd.nortel.vbk\r" \
"audio/vnd.nuera.ecelp4800			ecelp4800\r" \
"audio/vnd.nuera.ecelp7470			ecelp7470\r" \
"audio/vnd.nuera.ecelp9600			ecelp9600\r" \
"# audio/vnd.octel.sbc\r" \
"# audio/vnd.qcelp\r" \
"# audio/vnd.rhetorex.32kadpcm\r" \
"audio/vnd.rip					rip\r" \
"# audio/vnd.sealedmedia.softseal.mpeg\r" \
"# audio/vnd.vmx.cvsd\r" \
"# audio/vorbis\r" \
"# audio/vorbis-config\r" \
"audio/webm					weba\r" \
"audio/x-aac					aac\r" \
"audio/x-aiff					aif aiff aifc\r" \
"audio/x-caf					caf\r" \
"audio/x-flac					flac\r" \
"audio/x-matroska				mka\r" \
"audio/x-mpegurl					m3u\r" \
"audio/x-ms-wax					wax\r" \
"audio/x-ms-wma					wma\r" \
"audio/x-pn-realaudio				ram ra\r" \
"audio/x-pn-realaudio-plugin			rmp\r" \
"# audio/x-tta\r" \
"audio/x-wav					wav\r" \
"audio/xm					xm\r" \
"chemical/x-cdx					cdx\r" \
"chemical/x-cif					cif\r" \
"chemical/x-cmdf					cmdf\r" \
"chemical/x-cml					cml\r" \
"chemical/x-csml					csml\r" \
"# chemical/x-pdb\r" \
"chemical/x-xyz					xyz\r" \
"image/bmp					bmp\r" \
"image/cgm					cgm\r" \
"# image/example\r" \
"# image/fits\r" \
"image/g3fax					g3\r" \
"image/gif					gif\r" \
"image/ief					ief\r" \
"# image/jp2\r" \
"image/jpeg					jpeg jpg jpe\r" \
"# image/jpm\r" \
"# image/jpx\r" \
"image/ktx					ktx\r" \
"# image/naplps\r" \
"image/png					png\r" \
"image/prs.btif					btif\r" \
"# image/prs.pti\r" \
"image/sgi					sgi\r" \
"image/svg+xml					svg svgz\r" \
"# image/t38\r" \
"image/tiff					tiff tif\r" \
"# image/tiff-fx\r" \
"image/vnd.adobe.photoshop			psd\r" \
"# image/vnd.cns.inf2\r" \
"image/vnd.dece.graphic				uvi uvvi uvg uvvg\r" \
"image/vnd.dvb.subtitle				sub\r" \
"image/vnd.djvu					djvu djv\r" \
"image/vnd.dwg					dwg\r" \
"image/vnd.dxf					dxf\r" \
"image/vnd.fastbidsheet				fbs\r" \
"image/vnd.fpx					fpx\r" \
"image/vnd.fst					fst\r" \
"image/vnd.fujixerox.edmics-mmr			mmr\r" \
"image/vnd.fujixerox.edmics-rlc			rlc\r" \
"# image/vnd.globalgraphics.pgb\r" \
"# image/vnd.microsoft.icon\r" \
"# image/vnd.mix\r" \
"image/vnd.ms-modi				mdi\r" \
"image/vnd.ms-photo				wdp\r" \
"image/vnd.net-fpx				npx\r" \
"# image/vnd.radiance\r" \
"# image/vnd.sealed.png\r" \
"# image/vnd.sealedmedia.softseal.gif\r" \
"# image/vnd.sealedmedia.softseal.jpg\r" \
"# image/vnd.svf\r" \
"image/vnd.wap.wbmp				wbmp\r" \
"image/vnd.xiff					xif\r" \
"image/webp					webp\r" \
"image/x-3ds					3ds\r" \
"image/x-cmu-raster				ras\r" \
"image/x-cmx					cmx\r" \
"image/x-freehand				fh fhc fh4 fh5 fh7\r" \
"image/x-icon					ico\r" \
"image/x-mrsid-image				sid\r" \
"image/x-pcx					pcx\r" \
"image/x-pict					pic pct\r" \
"image/x-portable-anymap				pnm\r" \
"image/x-portable-bitmap				pbm\r" \
"image/x-portable-graymap			pgm\r" \
"image/x-portable-pixmap				ppm\r" \
"image/x-rgb					rgb\r" \
"image/x-tga					tga\r" \
"image/x-xbitmap					xbm\r" \
"image/x-xpixmap					xpm\r" \
"image/x-xwindowdump				xwd\r" \
"# message/cpim\r" \
"# message/delivery-status\r" \
"# message/disposition-notification\r" \
"# message/example\r" \
"# message/external-body\r" \
"# message/feedback-report\r" \
"# message/global\r" \
"# message/global-delivery-status\r" \
"# message/global-disposition-notification\r" \
"# message/global-headers\r" \
"# message/http\r" \
"# message/imdn+xml\r" \
"# message/news\r" \
"# message/partial\r" \
"message/rfc822					eml mime\r" \
"# message/s-http\r" \
"# message/sip\r" \
"# message/sipfrag\r" \
"# message/tracking-status\r" \
"# message/vnd.si.simp\r" \
"# model/example\r" \
"model/iges					igs iges\r" \
"model/mesh					msh mesh silo\r" \
"model/vnd.collada+xml				dae\r" \
"model/vnd.dwf					dwf\r" \
"# model/vnd.flatland.3dml\r" \
"model/vnd.gdl					gdl\r" \
"# model/vnd.gs-gdl\r" \
"# model/vnd.gs.gdl\r" \
"model/vnd.gtw					gtw\r" \
"# model/vnd.moml+xml\r" \
"model/vnd.mts					mts\r" \
"# model/vnd.parasolid.transmit.binary\r" \
"# model/vnd.parasolid.transmit.text\r" \
"model/vnd.vtu					vtu\r" \
"model/vrml					wrl vrml\r" \
"model/x3d+binary				x3db x3dbz\r" \
"model/x3d+vrml					x3dv x3dvz\r" \
"model/x3d+xml					x3d x3dz\r" \
"# multipart/alternative\r" \
"# multipart/appledouble\r" \
"# multipart/byteranges\r" \
"# multipart/digest\r" \
"# multipart/encrypted\r" \
"# multipart/example\r" \
"# multipart/form-data\r" \
"# multipart/header-set\r" \
"# multipart/mixed\r" \
"# multipart/parallel\r" \
"# multipart/related\r" \
"# multipart/report\r" \
"# multipart/signed\r" \
"# multipart/voice-message\r" \
"# text/1d-interleaved-parityfec\r" \
"text/cache-manifest				appcache\r" \
"text/calendar					ics ifb\r" \
"text/css					css\r" \
"text/csv					csv\r" \
"# text/directory\r" \
"# text/dns\r" \
"# text/ecmascript\r" \
"# text/enriched\r" \
"# text/example\r" \
"# text/fwdred\r" \
"text/html					html htm\r" \
"# text/javascript\r" \
"text/n3						n3\r" \
"# text/parityfec\r" \
"text/plain					txt text conf def list log in\r" \
"# text/prs.fallenstein.rst\r" \
"text/prs.lines.tag				dsc\r" \
"# text/vnd.radisys.msml-basic-layout\r" \
"# text/red\r" \
"# text/rfc822-headers\r" \
"text/richtext					rtx\r" \
"# text/rtf\r" \
"# text/rtp-enc-aescm128\r" \
"# text/rtx\r" \
"text/sgml					sgml sgm\r" \
"# text/t140\r" \
"text/tab-separated-values			tsv\r" \
"text/troff					t tr roff man me ms\r" \
"text/turtle					ttl\r" \
"# text/ulpfec\r" \
"text/uri-list					uri uris urls\r" \
"text/vcard					vcard\r" \
"# text/vnd.abc\r" \
"text/vnd.curl					curl\r" \
"text/vnd.curl.dcurl				dcurl\r" \
"text/vnd.curl.scurl				scurl\r" \
"text/vnd.curl.mcurl				mcurl\r" \
"# text/vnd.dmclientscript\r" \
"text/vnd.dvb.subtitle				sub\r" \
"# text/vnd.esmertec.theme-descriptor\r" \
"text/vnd.fly					fly\r" \
"text/vnd.fmi.flexstor				flx\r" \
"text/vnd.graphviz				gv\r" \
"text/vnd.in3d.3dml				3dml\r" \
"text/vnd.in3d.spot				spot\r" \
"# text/vnd.iptc.newsml\r" \
"# text/vnd.iptc.nitf\r" \
"# text/vnd.latex-z\r" \
"# text/vnd.motorola.reflex\r" \
"# text/vnd.ms-mediapackage\r" \
"# text/vnd.net2phone.commcenter.command\r" \
"# text/vnd.si.uricatalogue\r" \
"text/vnd.sun.j2me.app-descriptor		jad\r" \
"# text/vnd.trolltech.linguist\r" \
"# text/vnd.wap.si\r" \
"# text/vnd.wap.sl\r" \
"text/vnd.wap.wml				wml\r" \
"text/vnd.wap.wmlscript				wmls\r" \
"text/x-asm					s asm\r" \
"text/x-c					c cc cxx cpp h hh dic\r" \
"text/x-fortran					f for f77 f90\r" \
"text/x-java-source				java\r" \
"text/x-opml					opml\r" \
"text/x-pascal					p pas\r" \
"text/x-nfo					nfo\r" \
"text/x-setext					etx\r" \
"text/x-sfv					sfv\r" \
"text/x-uuencode					uu\r" \
"text/x-vcalendar				vcs\r" \
"text/x-vcard					vcf\r" \
"# text/xml\r" \
"# text/xml-external-parsed-entity\r" \
"# video/1d-interleaved-parityfec\r" \
"video/3gpp					3gp\r" \
"# video/3gpp-tt\r" \
"video/3gpp2					3g2\r" \
"# video/bmpeg\r" \
"# video/bt656\r" \
"# video/celb\r" \
"# video/dv\r" \
"# video/example\r" \
"video/h261					h261\r" \
"video/h263					h263\r" \
"# video/h263-1998\r" \
"# video/h263-2000\r" \
"video/h264					h264\r" \
"# video/h264-rcdo\r" \
"# video/h264-svc\r" \
"video/jpeg					jpgv\r" \
"# video/jpeg2000\r" \
"video/jpm					jpm jpgm\r" \
"video/mj2					mj2 mjp2\r" \
"# video/mp1s\r" \
"# video/mp2p\r" \
"# video/mp2t\r" \
"video/mp4					mp4 mp4v mpg4\r" \
"# video/mp4v-es\r" \
"video/mpeg					mpeg mpg mpe m1v m2v\r" \
"# video/mpeg4-generic\r" \
"# video/mpv\r" \
"# video/nv\r" \
"video/ogg					ogv\r" \
"# video/parityfec\r" \
"# video/pointer\r" \
"video/quicktime					qt mov\r" \
"# video/raw\r" \
"# video/rtp-enc-aescm128\r" \
"# video/rtx\r" \
"# video/smpte292m\r" \
"# video/ulpfec\r" \
"# video/vc1\r" \
"# video/vnd.cctv\r" \
"video/vnd.dece.hd				uvh uvvh\r" \
"video/vnd.dece.mobile				uvm uvvm\r" \
"# video/vnd.dece.mp4\r" \
"video/vnd.dece.pd				uvp uvvp\r" \
"video/vnd.dece.sd				uvs uvvs\r" \
"video/vnd.dece.video				uvv uvvv\r" \
"# video/vnd.directv.mpeg\r" \
"# video/vnd.directv.mpeg-tts\r" \
"# video/vnd.dlna.mpeg-tts\r" \
"video/vnd.dvb.file				dvb\r" \
"video/vnd.fvt					fvt\r" \
"# video/vnd.hns.video\r" \
"# video/vnd.iptvforum.1dparityfec-1010\r" \
"# video/vnd.iptvforum.1dparityfec-2005\r" \
"# video/vnd.iptvforum.2dparityfec-1010\r" \
"# video/vnd.iptvforum.2dparityfec-2005\r" \
"# video/vnd.iptvforum.ttsavc\r" \
"# video/vnd.iptvforum.ttsmpeg2\r" \
"# video/vnd.motorola.video\r" \
"# video/vnd.motorola.videop\r" \
"video/vnd.mpegurl				mxu m4u\r" \
"video/vnd.ms-playready.media.pyv		pyv\r" \
"# video/vnd.nokia.interleaved-multimedia\r" \
"# video/vnd.nokia.videovoip\r" \
"# video/vnd.objectvideo\r" \
"# video/vnd.sealed.mpeg1\r" \
"# video/vnd.sealed.mpeg4\r" \
"# video/vnd.sealed.swf\r" \
"# video/vnd.sealedmedia.softseal.mov\r" \
"video/vnd.uvvu.mp4				uvu uvvu\r" \
"video/vnd.vivo					viv\r" \
"video/webm					webm\r" \
"video/x-f4v					f4v\r" \
"video/x-fli					fli\r" \
"video/x-flv					flv\r" \
"video/x-m4v					m4v\r" \
"video/x-matroska				mkv mk3d mks\r" \
"video/x-mng					mng\r" \
"video/x-ms-asf					asf asx\r" \
"video/x-ms-vob					vob\r" \
"video/x-ms-wm					wm\r" \
"video/x-ms-wmv					wmv\r" \
"video/x-ms-wmx					wmx\r" \
"video/x-ms-wvx					wvx\r" \
"video/x-msvideo					avi\r" \
"video/x-sgi-movie				movie\r" \
"video/x-smv					smv\r" \
"x-conference/x-cooltalk				ice\r";

static map<string,string> createMimeMap(const string& unparsedMimeTypes) {
    map<string,string> m;

    ofBuffer buffer;
    buffer.set(unparsedMimeTypes);
    
    RegularExpression tabEater("[\t]+");
    RegularExpression spaceEater("[ ]+");
    
    while(!buffer.isLastLine()) {
        string line = buffer.getNextLine();
        if(line.empty() || line[0] == '#') continue;
        
        tabEater.subst(line, "\t"); // collapse tabs
        
        vector<string> tokens = ofSplitString(line,"\t");
        
        if(tokens.size() == 2 && !tokens[0].empty() && !tokens[1].empty()) {
            string mimeType = tokens[0];
//            int i = spaceEater.subst(tokens[1], " "); // collapse spaces
            vector<string> fileEndings = ofSplitString(tokens[1], " ");
            vector<string>::iterator iter = fileEndings.begin();
            while(iter != fileEndings.end()) {
                m[*iter] = mimeType;
                ++iter;
            }
        } else {
            ofLogWarning("ofxHTTPServer::loadDefaultMimeTypes") << "Unknown line format: " << line << ". Skipping.";
        }
    }

    return m;
    
}

static map<string, vector<string> > createFileTypeMap(const string& unparsedMimeTypes) {
    map<string, vector<string> > m;
    
    ofBuffer buffer;
    buffer.set(unparsedMimeTypes);
    
    RegularExpression tabEater("[\t]+");
    RegularExpression spaceEater("[ ]+");
    
    while(!buffer.isLastLine()) {
        string line = buffer.getNextLine();
        if(line.empty() || line[0] == '#') continue;
        
        tabEater.subst(line, "\t"); // collapse tabs
                
        vector<string> tokens = ofSplitString(line,"\t");
        
        if(tokens.size() == 2 && !tokens[0].empty() && !tokens[1].empty()) {
            string mimeType = tokens[0];
            spaceEater.subst(tokens[1], " "); // collapse spaces
            m[mimeType] = ofSplitString(tokens[1], " ");
        } else {
            ofLogWarning("ofxHTTPServer::loadDefaultMimeTypes") << "Unknown line format: " << line << ". Skipping.";
        }
    }
    
    
    return m;
    
}

static map<string,string> mimeTypeMap = createMimeMap(rawMimeTypes);
static map<string, vector<string> > fileTypeMap = createFileTypeMap(rawMimeTypes);

inline void ofxHTTPLoadMimeTypesFromFile(const string& mimeTypesFile) {
    mimeTypeMap = createMimeMap(ofBufferFromFile(mimeTypesFile));
    fileTypeMap = createFileTypeMap(ofBufferFromFile(mimeTypesFile));
}

inline MediaType ofxHTTPGetMimeType(const string& fileExtension) {
    map<string,string>::iterator iter = mimeTypeMap.find(fileExtension);
    if(iter != mimeTypeMap.end()) {
        return MediaType(iter->second);
    } else {
        ofLogWarning("ofxHTTPServerGetMimeType") << "Unmapped file extension: " << fileExtension;
        return MediaType("text/html");
    }
}

inline vector<string> ofxHTTPGetFileExtensionsForMimeType(const MediaType& mimeType) {
    vector<string> fileExtensions;
    map<string,vector<string> >::iterator iter = fileTypeMap.find(mimeType.getType());
    if(iter != fileTypeMap.end()) {
        fileExtensions = iter->second;
    } else {
        ofLogWarning("ofxHTTPServerGetFileExtensionsForMimeType") << "Unmapped mime type: " << mimeType.toString();
    }
    return fileExtensions;
}
