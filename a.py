import krunner
from PyQt5 import QtGui


class Runner(krunner.AbstractRunner):
    def __init__(self, *args):
        super().__init__(*args)
        self.a = [self.addAction("a", QtGui.QIcon.fromTheme("text-x-haskell"), "Haskell"),
                  self.addAction("b", QtGui.QIcon.fromTheme("text-x-python"), "Python"),
                  self.addAction("c", QtGui.QIcon.fromTheme("text-x-pascal"), "Pascal"),
                  self.addAction("d", QtGui.QIcon.fromTheme("text-x-java"), "Java")]

    def match(self, context):
        m = krunner.QueryMatch(self)
        m.setText("Hello from a " + context.query())
        m.setIconName("text-x-python")
        context.addMatch(m)

    def reloadConfiguration(self):
        print("CE", self.config().exists())

    def run(self, context, match):
        print("Text", match.text())
        print("Action selected:", match.selectedAction().text())

    def categories(self):
        return [" Awesome", " Ancient", " Awful", " Abstruse"]

    def actionsForMatch(self, match):
        return self.a

    def categoryIcon(self, name):
        if name == " Awesome":
            return QtGui.QIcon.fromTheme("text-x-python")
        elif name == " Ancient":
            return QtGui.QIcon.fromTheme("text-x-pascal")
        elif name == " Abstruse":
            return QtGui.QIcon.fromTheme("text-x-haskell")
        else:
            return QtGui.QIcon.fromTheme("text-x-java")

