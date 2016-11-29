import _krunner

import abc
from PyQt5 import QtCore

__all__ = ["AbstractRunner", "QueryMatch", "RunnerContext", "RunnerSyntax"]

for name in __all__[1:]:
    cls = getattr(_krunner.Plasma, name)
    globals()[name] = cls
    cls.__module__ = __name__

del name
del cls


class ARMeta(QtCore.pyqtWrapperType, abc.ABCMeta):
    pass


class AbstractRunner(_krunner.Plasma.AbstractRunner, metaclass=ARMeta):
    @abc.abstractmethod
    def match(self, runnercontext):
        pass
