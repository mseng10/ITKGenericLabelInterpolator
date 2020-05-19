ITKGenericLabelInterpolator
===========================

.. image:: https://github.com/InsightSoftwareConsortium/ITKGenericLabelInterpolator/workflows/Build,%20test,%20package/badge.svg
    :alt:    Build Status

Overview
--------

This is a module for the `Insight Toolkit (ITK) <http://itk.org>`_ that
provides a generic interpolator for label images to interpolate each label
with an ordinary image interpolator, and return the label with the highest
value. This is the idea used by the
`itk::LabelImageGaussianInterpolateImageFunction` interpolator. Unfortunately,
this class is currently limited to Gaussian interpolation. Using generic
programming, the proposed interpolator extends this idea to any image
interpolator. Combined with linear interpolation, this results in similar or
better accuracy and much improved computation speeds on a test image.

For more information, see the `Insight Journal article <http://hdl.handle.net/10380/3506>`_::

  Schaerer, J., Roche, F., Belaroussi, B.
  A generic interpolator for multi-label images
  The Insight Journal. January-December, 2014.
  http://hdl.handle.net/10380/3506
  http://www.insight-journal.org/browse/publication/950


Installation
------------

Since ITK 4.10.0, this module is available in the ITK source tree as a Remote
module.  To enable it, set::

  Module_GenericLabelInterpolator:BOOL=ON

in ITK's CMake build configuration.


License
-------

This software is distributed under the Apache 2.0 license. Please see the
*LICENSE* file for details.
