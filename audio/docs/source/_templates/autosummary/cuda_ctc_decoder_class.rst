..
  autogenerated from source/_templates/autosummary/cuda_ctc_decoder_class.rst


{#
    ################################################################################
    # autosummary template for CUCTCDecoder
    # Since the class has multiple methods and support structure.
    # we want to have them show up in the table of contents.
    # The default class template does not do this, so we use custom one here.
    ################################################################################
#}

{{ name | underline }}

{%- if name != "CUCTCDecoder" %}

.. autofunction:: {{fullname}}

{%- else %}

.. autoclass:: {{ fullname }}()

Methods
=======

{%- for item in members %}
{%- if not item.startswith('_') or item == "__call__" %}

{{ item | underline("-") }}

.. container:: py attribute

   .. automethod:: {{[fullname, item] | join('.')}}

{%- endif %}
{%- endfor %}

Support Structures
==================

{%- for item in ["CUCTCHypothesis"] %}

{{ item | underline("-") }}

.. autoclass:: torchaudio.models.decoder.{{item}}
   :members:

{%- endfor %}

{%- endif %}
