# Copyright (C) 2019 Red Hat, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

FROM debian:testing-slim as builder

RUN mkdir -p /output/bindep
RUN apt-get update && apt-get install -y python3-pip git && pip3 install bindep
COPY bindep.txt /bindep.txt
RUN cd / && bindep -l newline > /output/bindep/run.txt
RUN apt-get install -y $(bindep -b compile)
COPY . /src
RUN cd /src \
  && cargo build --release

FROM debian:testing-slim

COPY --from=builder /output/bindep/run.txt /run.txt
RUN apt-get update \
  && apt-get install -y dumb-init apache2 $(cat /run.txt) \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* /run.txt \
  && a2enmod rewrite proxy proxy_http
COPY ./vhost.conf /etc/apache2/sites-available/000-default.conf
COPY --from=builder /src/target/release/zuul_preview /usr/local/bin/zuul-preview

EXPOSE 80
ENTRYPOINT ["/usr/bin/dumb-init", "--"]
ENV ZUUL_API_URL=
CMD ["/usr/sbin/apachectl", "-DFOREGROUND", "-e", "info"]
