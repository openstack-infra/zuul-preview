============
zuul-preview
============

A preview proxy server for Zuul.

For example, when this http proxy is fed with a ``Host`` header in the form
similar to ``site.167715b656ee4504baa940c5bd9f3821.openstack.preview.opendev.org``
it proxies to the artifact 'site' of the build 167715b656ee4504baa940c5bd9f3821
under tenant 'openstack'. The actual location is determined dynamically by querying
a Zuul API configured by the environment variable ``$ZUUL_API_URL``.

Building
--------

First you need some dependencies:

.. code-block:: bash

  python3 -m pip install bindep
  apt-get install $(bindep -b compile)

Then you can build the code:

.. code-block:: bash

  autoreconf -fi
  ./configure
  make