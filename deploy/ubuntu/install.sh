#!/usr/bin/env bash
# Installs rythme on the Ubuntu box from an uploaded folder (deploy_ubuntu.ps1 sends it
# to /tmp/rythme-upload): the web build under /opt/rythme/web, Caddy's snippet in
# /etc/caddy/sites/kazin.dev.d/ (Caddy restarted only when the snippet changed: its
# admin API is off). No service: the page is static.
#   sudo bash install.sh [<upload folder>]
set -euo pipefail
SRC="${1:-/tmp/rythme-upload}"
SNIPPET=/etc/caddy/sites/kazin.dev.d/rythme.caddy

install -d -m 755 /opt/rythme

# The web build swapped whole (a visitor never gets half of two builds)
rm -rf /opt/rythme/web.new
cp -r "$SRC/web" /opt/rythme/web.new
chmod -R a+rX /opt/rythme/web.new
rm -rf /opt/rythme/web.old
[ -d /opt/rythme/web ] && mv /opt/rythme/web /opt/rythme/web.old
mv /opt/rythme/web.new /opt/rythme/web
rm -rf /opt/rythme/web.old

if ! cmp -s "$SRC/rythme.caddy" "$SNIPPET"; then
	[ -f "$SNIPPET" ] && cp "$SNIPPET" "$SNIPPET.bak"
	install -m 644 "$SRC/rythme.caddy" "$SNIPPET"
	if ! caddy validate --config /etc/caddy/Caddyfile --adapter caddyfile >/tmp/rythme-caddy.log 2>&1; then
		echo "(!) Caddy refused the snippet:"; tail -5 /tmp/rythme-caddy.log
		if [ -f "$SNIPPET.bak" ]; then mv -f "$SNIPPET.bak" "$SNIPPET"; else rm -f "$SNIPPET"; fi
		exit 1
	fi
	rm -f "$SNIPPET.bak"
	systemctl restart caddy
	echo "Caddy: snippet installed, restarted"
fi

ls -la /opt/rythme/web
