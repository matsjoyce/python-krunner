import krunner
from PyQt5 import QtGui


class Runner(krunner.AbstractRunner):
    def match(self, context):
        m = krunner.QueryMatch(self)
        m.setText("Hello from a " + context.query())
        m.setIconName("text-x-python")
        context.addMatch(m)

    def run(self, context, match):
        print(match.text())

    def categories(self):
        return ["a", "b"]

    def categoryIcon(self, name):
        print("NAME", name)
        return QtGui.QIcon.fromTheme("text-x-python")
