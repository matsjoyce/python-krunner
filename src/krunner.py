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
    # Public

    @abc.abstractmethod
    def match(self, runnercontext):
        pass

    def hasRunOptions(self):
        return self.parent().hasRunOptions()

    def speed(self):
        return self.parent().speed()

    def priority(self):
        return self.parent().priority()

    def ignoredTypes(self):
        return self.parent().ignoredTypes()

    def setIgnoredTypes(self, types):
        return self.parent().setIgnoredTypes(types)

    def name(self):
        return self.parent().name()

    def id(self):
        return self.parent().id()

    def description(self):
        return self.parent().description()

    def icon(self):
        return self.parent().icon()

    def syntaxes(self):
        return self.parent().syntaxes()

    def defaultSyntax(self):
        return self.parent().defaultSyntax()

    def isMatchingSuspended(self):
        return self.parent().isMatchingSuspended()

    # Private
    def suspendMatching(self, suspend):
        return self.parent().suspendMatching(suspend)

    def setHasRunOptions(self, hasRunOptions):
        return self.parent().setHasRunOptions(hasRunOptions)

    def setSpeed(self, newSpeed):
        return self.parent().setSpeed(newSpeed)

    def setPriority(self, newPriority):
        return self.parent().setPriority(newPriority)

    def addAction(self, id, icon_or_action, text=None):
        if text is None:
            return self.parent().addAction(id, icon_or_action)
        else:
            return self.parent().addAction(id, icon_or_action, text)

    def removeAction(self, id):
        return self.parent().removeAction(id)

    def action(self, id):
        return self.parent().action(id)

    def actions(self):
        return self.parent().actions()

    def clearActions(self):
        return self.parent().clearActions()

    def addSyntax(self, syntax):
        return self.parent().addSyntax(syntax)

    def setDefaultSyntax(self, syntax):
        return self.parent().setDefaultSyntax(syntax)

    def setSyntaxes(self, syns):
        return self.parent().setSyntaxes(syns)


def _except_hook(type, value, tb):
    # Used by plasma_runner_python to stop qFatal being called by PyQt5
    import traceback
    print("Exception in runner:")
    traceback.print_exception(type, value, tb)
