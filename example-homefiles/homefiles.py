import krunner
import os
import pathlib
import shlex
from PyQt5 import QtCore, QtWidgets


class Runner(krunner.AbstractRunner):
    def __init__(self, parent, args):
        super().__init__(parent, args)
        self.setIgnoredTypes(krunner.RunnerContext.NetworkLocation |
                             krunner.RunnerContext.Executable |
                             krunner.RunnerContext.ShellCommand)
        #self.setSpeed(krunner.AbstractRunner.SlowSpeed)
        #self.setPriority(krunner.AbstractRunner.LowPriority)
        self.setHasRunOptions(True)
        self._triggerWord = self._path = None

    def reloadConfiguration(self):
        c = self.config()
        self._triggerWord = c.readEntry("trigger", "")
        if self._triggerWord:
            self._triggerWord += " "

        self._path = pathlib.Path(c.readPathEntry("path", os.path.expanduser("~"))).resolve()

        if not self._path.is_dir():
            self._path = pathlib.Path(os.path.expanduser("~"))

        syntax = krunner.RunnerSyntax(("{}:q:").format(self._triggerWord),
                                      "Finds files matching :q: in the {} folder".format(self._path))
        self.setSyntaxes([syntax])

    def match(self, context):
        query = context.query()
        if query == "." or query == "..":
            return

        if not query.startswith(self._triggerWord):
            return

        query = query[len(self._triggerWord):]

        if len(query) > 2:
            query = "*{}*".format(query)

        for file in self._path.glob(query):
            if not context.isValid():
                return

            if self._path not in file.parents or file.name.startswith("."):
                continue

            match = krunner.QueryMatch(self)
            match.setText("Open {}".format(file))
            match.setData(str(file))
            match.setId(str(file))

            db = QtCore.QMimeDatabase()
            match.setIconName(db.mimeTypeForFile(str(file)).iconName())

            if str(self._path).lower() == query:
                match.setRelevance(1.0)
                match.setType(krunner.QueryMatch.ExactMatch)
            else:
                match.setRelevance(0.8)
            context.addMatch(match)

    def run(self, context, match):
        os.system("xdg-open {} &".format(shlex.quote(match.data())))

    def createRunOptions(self, widget):
        layout = QtWidgets.QVBoxLayout(widget)
        cb = QtWidgets.QCheckBox(widget)
        cb.setText("This is just for show")
        layout.addWidget(cb)
