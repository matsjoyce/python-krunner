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
    def __init__(self, parent, args):
        # Using parent() seems to remove the type
        self._parent = parent
        super().__init__(parent, args)

    # Public

    @abc.abstractmethod
    def match(self, runnercontext):
        pass

    def hasRunOptions(self):
        return self._parent.hasRunOptions()

    def speed(self):
        return self._parent.speed()

    def priority(self):
        return self._parent.priority()

    def ignoredTypes(self):
        return self._parent.ignoredTypes()

    def setIgnoredTypes(self, types):
        return self._parent.setIgnoredTypes(_krunner.Plasma.RunnerContext.Types(types))

    def name(self):
        return self._parent.name()

    def id(self):
        return self._parent.id()

    def description(self):
        return self._parent.description()

    def icon(self):
        return self._parent.icon()

    def syntaxes(self):
        return self._parent.syntaxes()

    def defaultSyntax(self):
        return self._parent.defaultSyntax()

    def isMatchingSuspended(self):
        return self._parent.isMatchingSuspended()

    # Private
    def suspendMatching(self, suspend):
        return self._parent.suspendMatching(suspend)

    def setHasRunOptions(self, hasRunOptions):
        return self._parent.setHasRunOptions(hasRunOptions)

    def setSpeed(self, newSpeed):
        return self._parent.setSpeed(newSpeed)

    def setPriority(self, newPriority):
        return self._parent.setPriority(newPriority)

    def addAction(self, id, icon_or_action, text=None):
        if text is None:
            return self._parent.addAction(id, icon_or_action)
        else:
            return self._parent.addAction(id, icon_or_action, text)

    def removeAction(self, id):
        return self._parent.removeAction(id)

    def action(self, id):
        return self._parent.action(id)

    def actions(self):
        return self._parent.actions()

    def clearActions(self):
        return self._parent.clearActions()

    def addSyntax(self, syntax):
        return self._parent.addSyntax(syntax)

    def setDefaultSyntax(self, syntax):
        return self._parent.setDefaultSyntax(syntax)

    def setSyntaxes(self, syns):
        return self._parent.setSyntaxes(syns)


def _except_hook(type, value, tb):
    # Used by plasma_runner_python to stop qFatal being called by PyQt5
    import traceback
    print("Exception in runner:")
    traceback.print_exception(type, value, tb)
