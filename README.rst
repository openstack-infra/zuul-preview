============
zuul-preview
============

A preview proxy server for zuul.

Building
--------

First you need some dependencies:

.. code-block:: bash

  python3 -m pip install bindep
  apt-get install $(bindep -b compile)

Then you can build the code:

.. code-block:: bash

  cargo build
